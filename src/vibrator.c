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
#ifdef WEARABLE
#include "feedback-ids-wearable.h"
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

#define METHOD_OPEN                 "OpenDevice"
#define METHOD_CLOSE                "CloseDevice"
#define METHOD_VIBRATE_EFFECT       "VibrateEffect"
#define METHOD_STOP                 "StopDevice"
#define METHOD_IS_SUPPORTED         "IsSupported"
#define METHOD_GET_EFFECT           "GetEffect"

#define DEFAULT_DURATION            100
#define SIP_DURATION                60
#define WHITESPACE      " \t"

static int vibstatus;
static unsigned int v_handle;

static inline char *trim_str(char *s)
{
	char *t;
	/* left trim */
	s += strspn(s, WHITESPACE);

	/* right trim */
	for (t = strchr(s, 0); t > s; t--)
		if (!strchr(WHITESPACE, t[-1]))
			break;
	*t = 0;
	return s;
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

	return dbus_method_sync(VIBRATOR_BUS_NAME, VIBRATOR_PATH_HAPTIC,
			VIBRATOR_INTERFACE_HAPTIC, METHOD_OPEN,
			"i", arr);
}

static int haptic_close(unsigned int handle)
{
	char *arr[1];
	char buf_handle[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;

	return dbus_method_sync(VIBRATOR_BUS_NAME, VIBRATOR_PATH_HAPTIC,
			VIBRATOR_INTERFACE_HAPTIC, METHOD_CLOSE,
			"u", arr);
}

static int haptic_is_supported(char *pattern)
{
	char *arr[1];

	arr[0] = pattern;

	return dbus_method_sync(VIBRATOR_BUS_NAME, VIBRATOR_PATH_HAPTIC,
			VIBRATOR_INTERFACE_HAPTIC, METHOD_IS_SUPPORTED,
			"s", arr);
}

static int haptic_vibrate_effect(unsigned int handle,
								char *pattern,
								int feedback,
								int priority)
{
	char *arr[4];
	char buf_handle[32];
	char buf_feedback[32];
	char buf_priority[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;
	arr[1] = pattern;
	snprintf(buf_feedback, sizeof(buf_feedback), "%d", feedback);
	arr[2] = buf_feedback;
	snprintf(buf_priority, sizeof(buf_priority), "%d", priority);
	arr[3] = buf_priority;

	return dbus_method_sync(VIBRATOR_BUS_NAME, VIBRATOR_PATH_HAPTIC,
			VIBRATOR_INTERFACE_HAPTIC, METHOD_VIBRATE_EFFECT,
			"usii", arr);
}

static int haptic_vibrate_stop(unsigned int handle)
{
	char *arr[1];
	char buf_handle[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;

	return dbus_method_sync(VIBRATOR_BUS_NAME, VIBRATOR_PATH_HAPTIC,
			VIBRATOR_INTERFACE_HAPTIC, METHOD_STOP,
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
	if (ret == -EACCES || ret == -ECOMM || ret == -EPERM) {
		_E("haptic_open ==> FAIL!! : %d", ret); //LCOV_EXCL_LINE
		v_handle = -EACCES; //LCOV_EXCL_LINE System Error
		return; //LCOV_EXCL_LINE System Error
	}
	if (ret < 0) {
		_E("haptic_open ==> FAIL!! : %d", ret); //LCOV_EXCL_LINE
		v_handle = -ENOTSUP; //LCOV_EXCL_LINE System Error
		return; //LCOV_EXCL_LINE System Error
	}

	/* Set vibration handle */
	v_handle = (unsigned int)ret;
}

static void vibrator_exit(void)
{
	int ret;

	if ((int)v_handle > 0) {
		ret = haptic_close(v_handle);
		if (ret < 0)
			_E("haptic_close is failed : %d", ret); //LCOV_EXCL_LINE
		v_handle = 0;
	}

}

static int vibrator_play(feedback_pattern_e pattern)
{
	char *temp;
	char *data;
	int ret;
	int level;

	if ((int)v_handle <= 0) {
		if (v_handle == 0) {
			_E("Not initialized"); //LCOV_EXCL_LINE
			return -ENOENT;
		}
		_E("Not supported vibration"); //LCOV_EXCL_LINE
		return v_handle; //LCOV_EXCL_LINE System Error
	}

	if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0) {
		_D("fail to get vibration status, will work as turning off"); //LCOV_EXCL_LINE
		vibstatus = 0;
	}

	if (vibstatus == 0 && profile->get_always_alert_case &&
	    !profile->get_always_alert_case(FEEDBACK_TYPE_VIBRATION, pattern))  {
		_D("Vibration condition is OFF (vibstatus : %d)", vibstatus); //LCOV_EXCL_LINE
		return 0;
	}

	if (vibstatus && profile->get_always_off_case &&
	    profile->get_always_off_case(FEEDBACK_TYPE_VIBRATION, pattern)) {
		_D("Vibration always off condition"); //LCOV_EXCL_LINE
		return 0;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Not supported vibration pattern"); //LCOV_EXCL_LINE
		return -ENOTSUP;
	}

	if (profile->get_strength_type)
		level = profile->get_strength_type(FEEDBACK_TYPE_VIBRATION, pattern);
	else
		level = DEFAULT_VIB_LEVEL * HAPTIC_FEEDBACK_STEP;

	temp = strdup(profile->str_pattern[pattern]);
	data = trim_str(temp);
	if (!data) {
		free(temp);
		_E("Not supported vibration pattern"); //LCOV_EXCL_LINE
		return -ENOTSUP;
	}

	_D("pattern : %s", data);

	ret = haptic_vibrate_effect(v_handle, data, level, get_priority(pattern));

	if (ret < 0) {
		_E("fail to play vibration"); //LCOV_EXCL_LINE
		free(temp);
		if (ret == -ECOMM || ret == -ENOTSUP)
			return ret; //LCOV_EXCL_LINE System Error
		return -EPERM;
	}

	_D("Play success! Data is %s", data);
	free(temp);
	return 0;
}

static int vibrator_stop(void)
{
	int ret;

	if ((int)v_handle <= 0) {
		if (v_handle == 0) {
			_E("Not initialized"); //LCOV_EXCL_LINE
			return -ENOENT;
		}
		_E("Not supported vibration"); //LCOV_EXCL_LINE
		return v_handle; //LCOV_EXCL_LINE System Error
	}

	/* stop haptic device */
	ret = haptic_vibrate_stop(v_handle);
	if (ret < 0) {
		_E("haptic_vibrate_stop is failed"); //LCOV_EXCL_LINE
		if (ret == -ECOMM)
			return ret; //LCOV_EXCL_LINE System Error
		return -EPERM;
	}

	return 0;
}

static int vibrator_is_supported(int pattern, bool *supported)
{
	char *temp;
	char *data;
	int ret = 0;

	if (!supported) {
		_E("Invalid parameter : supported(NULL)"); //LCOV_EXCL_LINE
		return -EINVAL;
	}

	if ((int)v_handle <= 0) {
		if (v_handle == 0) {
			_E("Not initialized"); //LCOV_EXCL_LINE
			return -ENOENT;
		}
		_E("Not supported vibration"); //LCOV_EXCL_LINE
		*supported = false; //LCOV_EXCL_LINE System Error
		return v_handle;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Not supported vibration pattern"); //LCOV_EXCL_LINE
		*supported = false;
		return 0;
	}

	/* get vibration data */
	temp = strdup(profile->str_pattern[pattern]);
	data = trim_str(temp);
	if (!data) {
		free(temp);
		*supported = false;
		return 0;
	}

	ret = haptic_is_supported(data);
	free(temp);
	if (ret < 0) {
		_E("fail to get support information"); //LCOV_EXCL_LINE
		if (ret == -ECOMM)
			return ret;
		return -EPERM;
	}

	_D("is_supported : %d", ret);
	*supported = ret;
	return 0;
}

static int vibrator_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	return 0;
}

static int vibrator_set_path(feedback_pattern_e pattern, char *path)
{
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
