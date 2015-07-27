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
static dd_list *vib_effect_head;

struct vibration_effect_info {
	char *pattern;
	char *effect_name;
};

static int vibrator_set_effect_name(feedback_pattern_e pattern, char *effect_name);
static int vibrator_get_effect_name(feedback_pattern_e pattern, char *buf, unsigned int buflen);

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

static int haptic_is_supported(char *pattern)
{
	char *arr[1];

	arr[0] = pattern;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_IS_SUPPORTED,
			"s", arr);
}

static int haptic_vibrate_effect(unsigned int handle,
								char *pattern,
								char *effect_name,
								int feedback,
								int priority)
{
	char *arr[5];
	char buf_handle[32];
	char buf_feedback[32];
	char buf_priority[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;
	arr[1] = pattern;
	arr[2] = effect_name;
	snprintf(buf_feedback, sizeof(buf_feedback), "%d", feedback);
	arr[3] = buf_feedback;
	snprintf(buf_priority, sizeof(buf_priority), "%d", priority);
	arr[4] = buf_priority;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_VIBRATE_EFFECT,
			"ussii", arr);
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
	dd_list *elem;
	struct vibration_effect_info *vib_einfo;

	if (v_handle > 0) {
		ret = haptic_close(v_handle);
		if (ret < 0)
			_E("haptic_close is failed : %d", ret);
		v_handle = 0;
	}

	DD_LIST_FOREACH(vib_effect_head, elem, vib_einfo)
		free(vib_einfo);

}

static int vibrator_play(feedback_pattern_e pattern)
{
	struct vibration_effect_info *vib_einfo;
	dd_list *elem;
	char *data;
	char *buf;
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

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Not supported vibration pattern");
		return -ENOTSUP;
	}

	if (profile->get_strength_type)
		level = profile->get_strength_type(FEEDBACK_TYPE_VIBRATION, pattern);
	else
		level = DEFAULT_VIB_LEVEL * HAPTIC_FEEDBACK_STEP;

	data = trim_str(strdup(profile->str_pattern[pattern]));
	if (!data) {
		_E("Not supported vibration pattern");
		return -ENOTSUP;
	}

	_D("pattern : %s", data);
	buf = malloc((strlen(data)+20) * sizeof(char));
	snprintf(buf, strlen(data)+20, "%s", "NULL");
	/* get vibration effect name  */
	DD_LIST_FOREACH(vib_effect_head, elem, vib_einfo) {
		if (!vib_einfo->pattern)
			continue;
		if (!strncmp(vib_einfo->pattern, profile->str_pattern[pattern],
				strlen(vib_einfo->pattern)+1))
			snprintf(buf, strlen(data)+20, "%s", vib_einfo->effect_name);
	}
	_D("buf : %s", buf);

	ret = haptic_vibrate_effect(v_handle, data, buf, level, get_priority(pattern));

	if (ret < 0) {
		_E("fail to play vibration");
		if (ret == -ECOMM)
			return ret;
		return -EPERM;
	}

	_D("Play success! Data is %s", data);
	free(buf);
	free(data);
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
	dd_list *elem;
	struct vibration_effect_info *vib_einfo;
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

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Not supported vibration pattern");
		*supported = false;
		return 0;
	}

	DD_LIST_FOREACH(vib_effect_head, elem, vib_einfo) {
		if (!vib_einfo->pattern)
			continue;
		if (!strncmp(vib_einfo->pattern, profile->str_pattern[pattern],
				strlen(vib_einfo->pattern)+1)) {
			*supported = true;
			return 0;
		}
	}

	/* get vibration data */
	data = trim_str(strdup(profile->str_pattern[pattern]));
	ret = haptic_is_supported(data);
	if (ret < 0) {
		_E("fail to get support information");
		if (ret == -ECOMM)
			return ret;
		return -EPERM;
	}

	_D("is_supported : %d", ret);
	*supported = ret;
	free(data);
	return 0;
}

static int vibrator_get_effect_name(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	dd_list *elem;
	struct vibration_effect_info *vib_einfo;
	DBusMessage *reply;
	char *arr[1];
	char *data;

	if (!buf || buflen <= 0)
		return -EINVAL;

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("This pattern(%s) in vibrator type is not supported to play",
				profile->str_pattern[pattern]);
		snprintf(buf, buflen, "%s", "NULL");
		return -ENOENT;
	}

	/* get vibration effect name  */
	DD_LIST_FOREACH(vib_effect_head, elem, vib_einfo) {
		if (!vib_einfo->pattern)
			continue;
		if (!strncmp(vib_einfo->pattern, profile->str_pattern[pattern],
				strlen(vib_einfo->pattern)+1)) {
			_D("match! pattern : %s effect : %s", vib_einfo->pattern, vib_einfo->effect_name);
			snprintf(buf, buflen, "%s", vib_einfo->effect_name);
			return 0;
		}
	}

	/*
	 * Need to use dbus call for effect name
	*/
	arr[0] = trim_str(strdup(profile->str_pattern[pattern]));
	reply = dbus_method_sync_with_reply(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_GET_EFFECT,
			"s", arr);
	if (!reply)
		return -EPERM;

	if(!dbus_message_get_args(reply, NULL, DBUS_TYPE_STRING, &data,
			DBUS_TYPE_INVALID)) {
		dbus_message_unref(reply);
		_E("error");
		snprintf(buf, buflen, "%s", "NULL");
		return -EPERM;
	}
	dbus_message_unref(reply);

	if (!data)
		snprintf(buf, buflen, "%s", "NULL");
	else
		snprintf(buf, buflen, "%s", data);
	_D("data : %s", buf);
	return 0;

}

static int vibrator_set_effect_name(feedback_pattern_e pattern, char *effect_name)
{
	dd_list *elem;
	struct vibration_effect_info *vib_einfo;

	/*
	 * check the effec_name exists.
	 * if effect_name is null, reset vibration path
	 */
	if (!effect_name) {
		DD_LIST_FOREACH(vib_effect_head, elem, vib_einfo) {
			if (!vib_einfo->pattern)
				continue;
			if (!strncmp(vib_einfo->pattern, profile->str_pattern[pattern],
					strlen(vib_einfo->pattern)+1)) {
				free(vib_einfo);
			}

		}
		return 0;
	}

	DD_LIST_FOREACH(vib_effect_head, elem, vib_einfo) {
		if (!vib_einfo->pattern)
			continue;
		if (!strncmp(vib_einfo->pattern, profile->str_pattern[pattern],
				strlen(vib_einfo->pattern)+1)) {
			free(vib_einfo->effect_name);
			vib_einfo->effect_name = trim_str(strdup(effect_name));
			_D("The file of pattern(%s) is changed to [%s]",
					vib_einfo->pattern, vib_einfo->effect_name);
			return 0;
		}
	}

	vib_einfo = (struct vibration_effect_info *)malloc(sizeof(struct vibration_effect_info));
	if (!vib_einfo) {
		_E("fail to alloc");
		return -ENOMEM;
	}

	vib_einfo->pattern = trim_str(strdup(profile->str_pattern[pattern]));
	vib_einfo->effect_name = trim_str(strdup(effect_name));

	DD_LIST_APPEND(vib_effect_head, vib_einfo);

	_D("The file of pattern(%s) is changed to [%s]",
			vib_einfo->pattern, vib_einfo->effect_name);
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
	.get_path = vibrator_get_effect_name,
	.set_path = vibrator_set_effect_name,
};

DEVICE_OPS_REGISTER(&vibrator_device_ops);
