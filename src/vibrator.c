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
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <vconf.h>
#include <sys/stat.h>

#include "feedback-ids.h"
#include "profiles.h"
#include "devices.h"
#include "log.h"
#include "dbus.h"

#define HAPTIC_DEVICE				0

enum haptic_priority
{
	HAPTIC_PRIORITY_MIN = 0,
	HAPTIC_PRIORITY_MIDDLE,
	HAPTIC_PRIORITY_HIGH,
};

enum haptic_iteration
{
	HAPTIC_ITERATION_ONCE = 1,
	HAPTIC_ITERATION_INFINITE = 256,
};

#define VIBRATION_XML               "/usr/share/feedback/vibration.xml"

#define METHOD_OPEN                 "OpenDevice"
#define METHOD_CLOSE                "CloseDevice"
#define METHOD_VIBRATE_MONOTONE     "VibrateMonotone"
#define METHOD_STOP                 "StopDevice"

#define DEFAULT_DURATION            100

static int vibstatus;

static unsigned int v_handle;

static char haptic_file[FEEDBACK_PATTERN_END][NAME_MAX];

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

static int get_priority(feedback_pattern_e pattern)
{
	if (pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD)
		return HAPTIC_PRIORITY_MIN;

	return HAPTIC_PRIORITY_MIDDLE;
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
}

static int vibrator_play(feedback_pattern_e pattern)
{
	int ret;
	int level;

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

	ret = haptic_vibrate_monotone(v_handle, DEFAULT_DURATION,
			level, get_priority(pattern));
	if (ret < 0) {
		_E("haptic_vibrate_monotone is failed");
		if (ret == -ECOMM)
			return ret;
		return -EPERM;
	}

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

	*supported = true;
	return 0;
}

static int vibrator_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	const char *cur_path;

	assert(buf != NULL && buflen > 0);

	cur_path = haptic_file[pattern];
	if (*cur_path) {
		_E("This pattern(%s) in vibrator type is not supported to play",
				profile->str_pattern[pattern]);
		snprintf(buf, buflen, "NULL");
		return -ENOENT;
	}

	snprintf(buf, buflen, "%s", cur_path);
	return 0;
}

static int vibrator_set_path(feedback_pattern_e pattern, char *path)
{
	struct stat buf;
	char *ppath;

	/*
	 * check the path is valid
	 * if path is null, below operation is ignored
	 */
	if (path && stat(path, &buf)) {
		_E("%s is not presents", path);
		return -errno;
	}

	ppath = haptic_file[pattern];

	/* if path is NULL, this pattern set to default file */
	if (path)
		snprintf(ppath, NAME_MAX, "%s", path);
	else
		memset(ppath, 0, NAME_MAX);

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
