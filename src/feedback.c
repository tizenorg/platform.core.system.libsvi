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
#include <limits.h>
#include <vconf.h>

#include "feedback.h"
#include "common.h"
#include "log.h"
#include "devices.h"

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

int callstatus;

static bool binit;

static void feedback_callstatus_cb(keynode_t *key, void* data)
{
	callstatus = vconf_keynode_get_int(key);
}

static feedback_pattern_e get_alert_on_call_key(feedback_pattern_e pattern)
{
	switch(pattern) {
	case FEEDBACK_PATTERN_MESSAGE:
	case FEEDBACK_PATTERN_EMAIL:
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_SCHEDULE:
	case FEEDBACK_PATTERN_TIMER:
	case FEEDBACK_PATTERN_GENERAL:
	case FEEDBACK_PATTERN_CHARGERCONN:
	case FEEDBACK_PATTERN_FULLCHARGED:
	case FEEDBACK_PATTERN_LOWBATT:
		return (feedback_pattern_e)(pattern+1);
	default:
		break;
	}

	return pattern;
}

API int feedback_initialize()
{
	if (binit)
		return FEEDBACK_ERROR_NONE;

	/* check call status */
	if (vconf_get_int(VCONFKEY_CALL_STATE, &callstatus) < 0)
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_CALL_STATE, &callstatus) ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb, NULL);

	/* initialize device */
	devices_init();

	binit = true;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_deinitialize()
{
	if (!binit)
		return FEEDBACK_ERROR_NOT_INITIALIZED;

	vconf_ignore_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb);

	/* deinitialize device */
	devices_exit();

	binit = false;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_play(feedback_pattern_e pattern)
{
	/* check initialize */
	if (!binit) {
		FEEDBACK_ERROR("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (pattern < FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern == FEEDBACK_PATTERN_NONE) {
		FEEDBACK_LOG("pattern is NONE");
		return FEEDBACK_ERROR_NONE;
	}

	/* in case of call connected or connecting */
	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
	}

	/* play all device */
	devices_play(pattern);

	return FEEDBACK_ERROR_NONE;
}

API int feedback_play_type(feedback_type_e type, feedback_pattern_e pattern)
{
	const struct device_ops *dev;
	int err;

	/* check initialize */
	if (!binit) {
		FEEDBACK_ERROR("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern < FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern == FEEDBACK_PATTERN_NONE) {
		FEEDBACK_LOG("pattern is NONE");
		return FEEDBACK_ERROR_NONE;
	}

	/* in case of call connected or connecting */
	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
	}

	/* play proper device */
	dev = find_device(type);
	if (dev) {
		err = dev->play(pattern);
		if (err < 0)
			FEEDBACK_ERROR("fail to play sound");
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_get_resource_path(feedback_type_e type, feedback_pattern_e pattern, char** path)
{
	const struct device_ops *dev;
	char buf[PATH_MAX] = {0,};
	int err;

	if (path == NULL) {
		FEEDBACK_ERROR("Invalid parameter : path(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	/* proper device get path */
	dev = find_device(type);
	if (dev) {
		err = dev->get_path(pattern, buf, sizeof(buf));
		if (err < 0)
			return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	*path = strdup(buf);
	return FEEDBACK_ERROR_NONE;
}

API int feedback_set_resource_path(feedback_type_e type, feedback_pattern_e pattern, char *path)
{
	const struct device_ops *dev;
	int err;

	if (path == NULL) {
		FEEDBACK_ERROR("Invalid parameter : path(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	/* proper device set path */
	dev = find_device(type);
	if (dev) {
		err = dev->set_path(pattern, path);
		if (err < 0)
			return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}
