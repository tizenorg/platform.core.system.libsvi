/*
 * libfeedback
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the License);
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <vconf.h>
#include <sys/stat.h>

#include "feedback-ids.h"
#include "profiles.h"
#include "parser.h"
#include "devices.h"
#include "log.h"
#include "dbus.h"
#ifdef MOBILE
#include "feedback-ids-mobile.h"
#endif

#define HAPTIC_DEVICE				0

enum haptic_priority {
	HAPTIC_PRIORITY_MIN = 0,
	HAPTIC_PRIORITY_MIDDLE,
	HAPTIC_PRIORITY_HIGH,
};

enum haptic_iteration {
	HAPTIC_ITERATION_ONCE = 1,
	HAPTIC_ITERATION_INFINITE = 256,
};

#define VIBRATION_CONF_FILE         "/usr/share/feedback/vibration.conf"

#define METHOD_OPEN                 "OpenDevice"
#define METHOD_CLOSE                "CloseDevice"
#define METHOD_VIBRATE_MONOTONE     "VibrateMonotone"
#define METHOD_VIBRATE_BUFFER       "VibrateBuffer"
#define METHOD_STOP                 "StopDevice"

#define DEFAULT_DURATION            100
#define SIP_DURATION                60

static int vibstatus;
static unsigned int v_handle;
static struct feedback_config_info vib_info = {
	.name = "Vibration",
};

static char *get_data(feedback_pattern_e pattern)
{
	char *data;

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern)
		return NULL;

	if (vib_info.data[pattern].changed)
		data = vib_info.data[pattern].changed;
	else
		data = vib_info.data[pattern].origin;

	return data;
}

inline int is_vibration_mode(void)
{
	return vibstatus;
}

static int haptic_open(void)
{
	char *arr[1];
	char buf_index[32];

	snprintf(buf_index, sizeof(buf_index), "%d", HAPTIC_DEVICE);
	arr[0] = buf_index;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_OPEN,
			"i", arr);
}

static int haptic_close(unsigned int handle)
{
	char *arr[1];
	char buf_handle[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_CLOSE,
			"u", arr);
}

static int haptic_vibrate_buffer(unsigned int handle,
								const unsigned char *buffer,
								int size,
								int iteration,
								int feedback,
								int priority)
{
	char *arr[6];
	char buf_handle[32];
	char buf_iteration[32];
	char buf_feedback[32];
	char buf_priority[32];
	struct dbus_byte bytes;

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;
	bytes.size = size;
	bytes.data = buffer;
	arr[2] = (char *)&bytes;
	snprintf(buf_iteration, sizeof(buf_iteration), "%d", iteration);
	arr[3] = buf_iteration;
	snprintf(buf_feedback, sizeof(buf_feedback), "%d", feedback);
	arr[4] = buf_feedback;
	snprintf(buf_priority, sizeof(buf_priority), "%d", priority);
	arr[5] = buf_priority;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_VIBRATE_BUFFER,
			"uayiii", arr);
}

static int haptic_vibrate_monotone(unsigned int handle,
								int duration,
								int feedback,
								int priority)
{
	char *arr[4];
	char buf_handle[32];
	char buf_duration[32];
	char buf_feedback[32];
	char buf_priority[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;
	snprintf(buf_duration, sizeof(buf_duration), "%d", duration);
	arr[1] = buf_duration;
	snprintf(buf_feedback, sizeof(buf_feedback), "%d", feedback);
	arr[2] = buf_feedback;
	snprintf(buf_priority, sizeof(buf_priority), "%d", priority);
	arr[3] = buf_priority;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_VIBRATE_MONOTONE,
			"uiii", arr);
}

static int haptic_vibrate_stop(unsigned int handle)
{
	char *arr[1];
	char buf_handle[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_STOP,
			"u", arr);
}

static unsigned char *convert_file_to_buffer(const char *file_name, int *size)
{
	FILE *pf;
	long file_size;
	unsigned char *pdata = NULL;

	if (!file_name)
		return NULL;

	/* Get File Stream Pointer */
	pf = fopen(file_name, "rb");
	if (!pf) {
		_E("fopen failed : %s", strerror(errno));
		return NULL;
	}

	if (fseek(pf, 0, SEEK_END))
		goto error;

	file_size = ftell(pf);
	if (fseek(pf, 0, SEEK_SET))
		goto error;

	if (file_size < 0)
		goto error;

	pdata = (unsigned char *)malloc(file_size);
	if (!pdata)
		goto error;

	if (fread(pdata, 1, file_size, pf) != file_size)
		goto err_free;

	fclose(pf);
	*size = file_size;
	return pdata;

err_free:
	free(pdata);

error:
	fclose(pf);

	_E("failed to convert file to buffer (%s)", strerror(errno));
	return NULL;
}

static int get_priority(feedback_pattern_e pattern)
{
	if (pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD)
		return HAPTIC_PRIORITY_MIN;

	return HAPTIC_PRIORITY_MIDDLE;
}

static int get_duration(feedback_pattern_e pattern)
{
	if (pattern == FEEDBACK_PATTERN_SIP)
		return SIP_DURATION;
#ifdef MOBILE
	if (pattern == (feedback_pattern_e)FEEDBACK_PATTERN_MOBILE_SIP_BACKSPACE)
		return SIP_DURATION;
#endif
#ifdef WEARABLE
	if (pattern == (feedback_pattern_e)FEEDBACK_PATTERN_WEARABLE_SIP_BACKSPACE)
		return SIP_DURATION;
#endif
	return DEFAULT_DURATION;
}

static void vibrator_init(void)
{
	int ret;

	/* Vibration Init */
	ret = haptic_open();
	if (ret < 0) {
		_E("haptic_open ==> FAIL!! : %d", ret);
		v_handle = -ENOTSUP;
		return;
	}

	/* Set vibration handle */
	v_handle = (unsigned int)ret;

	/* get vibration data */
	feedback_load_config(VIBRATION_CONF_FILE, &vib_info);
}

static void vibrator_exit(void)
{
	int ret;

	if (v_handle > 0) {
		ret = haptic_close(v_handle);
		if (ret < 0)
			_E("haptic_close is failed : %d", ret);
		v_handle = 0;
	}

	/* free vibration data */
	feedback_free_config(&vib_info);
}

static int vibrator_play(feedback_pattern_e pattern)
{
	struct stat buf;
	char *data;
	unsigned char *pbuf;
	int size;
	int ret;
	int level;
	int duration;

	if (!v_handle) {
		_E("Not initialize");
		return -EPERM;
	}

	if (v_handle == -ENOTSUP) {
		_E("Not supported vibration");
		return -ENOTSUP;
	}

	if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0) {
		_D("fail to get vibration status, will work as turning off");
		vibstatus = 0;
	}

	if (vibstatus == 0 && profile->get_always_alert_case &&
	    !profile->get_always_alert_case(FEEDBACK_TYPE_VIBRATION, pattern))  {
		_D("Vibration condition is OFF (vibstatus : %d)", vibstatus);
		return 0;
	}

	if (vibstatus && profile->get_always_off_case &&
	    profile->get_always_off_case(FEEDBACK_TYPE_VIBRATION, pattern)) {
		_D("Vibration always off condition");
		return 0;
	}

	if (profile->get_strength_type)
		level = profile->get_strength_type(FEEDBACK_TYPE_VIBRATION, pattern);
	else
		level = DEFAULT_VIB_LEVEL * HAPTIC_FEEDBACK_STEP;

	/* get vibration data */
	data = get_data(pattern);
	if (!data) {
		_E("Not supported vibration pattern");
		return -ENOTSUP;
	}

	/* if it has a file path */
	if (!stat(data, &buf)) {
		pbuf = convert_file_to_buffer(data, &size);
		if (!pbuf) {
			_E("fail to convert file to buffer");
			return -EPERM;
		}

		ret = haptic_vibrate_buffer(v_handle, pbuf, size,
				HAPTIC_ITERATION_ONCE,
				level, get_priority(pattern));
		free(pbuf);
	} else {
		duration = get_duration(pattern);
		ret = haptic_vibrate_monotone(v_handle, duration,
				level, get_priority(pattern));
	}

	if (ret < 0) {
		_E("fail to play vibration");
		if (ret == -ECOMM)
			return ret;
		return -EPERM;
	}

	_D("Play success! Data is %s", data);
	return 0;
}

static int vibrator_stop(void)
{
	int ret;

	if (!v_handle) {
		_E("Not initialize");
		return -EPERM;
	}

	if (v_handle == -ENOTSUP) {
		_E("Not supported vibration");
		return -ENOTSUP;
	}

	/* stop haptic device */
	ret = haptic_vibrate_stop(v_handle);
	if (ret < 0) {
		_E("haptic_vibrate_stop is failed");
		if (ret == -ECOMM)
			return ret;
		return -EPERM;
	}

	return 0;
}

static int vibrator_is_supported(int pattern, bool *supported)
{
	char *data;
	bool ret = true;

	if (!supported) {
		_E("Invalid parameter : supported(NULL)");
		return -EINVAL;
	}

	if (!v_handle) {
		_E("Not initialize");
		return -EPERM;
	}

	if (v_handle == -ENOTSUP) {
		_E("Not supported vibration");
		*supported = false;
		return 0;
	}

	/* get vibration data */
	data = get_data(pattern);
	if (!data) {
		_E("Not supported vibration pattern");
		ret = false;
	}

	*supported = ret;
	return 0;
}

static int vibrator_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	char *data;
	int ret = 0;

	if (!buf || buflen <= 0)
		return -EINVAL;

	/* get vibration data */
	data = get_data(pattern);
	if (!data) {
		_E("This pattern(%s) in vibrator type is not supported to play",
				profile->str_pattern[pattern]);
		data = "NULL";
		ret = -ENOENT;
	}

	snprintf(buf, buflen, "%s", data);
	return ret;
}

static int vibrator_set_path(feedback_pattern_e pattern, char *path)
{
	struct stat buf;

	/*
	 * check the path is valid
	 * if path is null, reset vibration path
	 */
	if (!path) {
		if (vib_info.data[pattern].changed) {
			free(vib_info.data[pattern].changed);
			vib_info.data[pattern].changed = NULL;
		}
		return 0;
	}

	if (path && stat(path, &buf)) {
		_E("%s is not presents", path);
		return -errno;
	}

	if (vib_info.data[pattern].changed) {
		free(vib_info.data[pattern].changed);
		vib_info.data[pattern].changed = NULL;
	}

	/* if path is NULL, this pattern set to default file */
	if (path)
		vib_info.data[pattern].changed = strdup(path);

	_D("The file of pattern(%s) is changed to [%s]",
			profile->str_pattern[pattern], path);
	return 0;
}

static const struct device_ops vibrator_device_ops = {
	.type = FEEDBACK_TYPE_VIBRATION,
	.name = "Vibrator",
	.init = vibrator_init,
	.exit = vibrator_exit,
	.play = vibrator_play,
	.stop = vibrator_stop,
	.is_supported = vibrator_is_supported,
	.get_path = vibrator_get_path,
	.set_path = vibrator_set_path,
};

DEVICE_OPS_REGISTER(&vibrator_device_ops);
