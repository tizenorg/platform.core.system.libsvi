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
int alert_callstatus;

static bool binit;

static void feedback_callstatus_cb(keynode_t *key, void* data)
{
	callstatus = vconf_keynode_get_int(key);
}

static void feedback_alertstatus_cb(keynode_t *key, void* data)
{
	alert_callstatus = vconf_keynode_get_int(key);
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
	case FEEDBACK_PATTERN_CHARGING_ERROR:
	case FEEDBACK_PATTERN_FULLCHARGED:
	case FEEDBACK_PATTERN_LOWBATT:
		return (feedback_pattern_e)(pattern+1);
	default:
		break;
	}

	return pattern;
}

static void __DESTRUCTOR__ module_exit(void)
{
	if (!binit)
		return;

	vconf_ignore_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_CISSAPPL_ALERT_ON_CALL_INT, feedback_alertstatus_cb);

	/* deinitialize device */
	devices_exit();

	binit = false;
}

API int feedback_initialize()
{
	if (binit)
		return FEEDBACK_ERROR_NONE;

	/* check call status */
	if (vconf_get_int(VCONFKEY_CALL_STATE, &callstatus) < 0)
		_W("VCONFKEY_CALL_STATE ==> FAIL!!");


	/* alert option on call */
	if (vconf_get_int(VCONFKEY_CISSAPPL_ALERT_ON_CALL_INT, &alert_callstatus) < 0)
		_W("VCONFKEY_CISSAPPL_ON_CALL_INT ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_CISSAPPL_ALERT_ON_CALL_INT, feedback_alertstatus_cb, NULL);

	/* initialize device */
	devices_init();

	binit = true;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_deinitialize()
{
	return FEEDBACK_ERROR_NONE;
}

API int feedback_play(feedback_pattern_e pattern)
{
	/* check initialize */
	if (!binit) {
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (pattern < FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern == FEEDBACK_PATTERN_NONE) {
		_D("pattern is NONE");
		return FEEDBACK_ERROR_NONE;
	}

	/* in case of call connected or connecting */
	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = get_alert_on_call_key(pattern);
		_D("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
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
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern < FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern == FEEDBACK_PATTERN_NONE) {
		_D("pattern is NONE");
		return FEEDBACK_ERROR_NONE;
	}

	/* in case of call connected or connecting */
	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = get_alert_on_call_key(pattern);
		_D("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
	}

	/* should play led regardless of sound or vibration */
	dev = find_device(FEEDBACK_TYPE_LED);
	if (dev) {
		err = dev->play(pattern);
		if (err < 0)
			_E("feedback_play_led is failed");
	}

	if (type == FEEDBACK_TYPE_LED)
		return FEEDBACK_ERROR_NONE;

	/* play proper device */
	dev = find_device(type);
	if (dev) {
		err = dev->play(pattern);
		if (err < 0)
			_E("fail to play sound");
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_play_type_by_name(char *type, char *pattern)
{
	feedback_type_e etype;
	feedback_pattern_e epattern;

	if (!type || !pattern) {
		_E("Invalid parameter : type(%x), pattern(%x)", type, pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	for (etype = FEEDBACK_TYPE_NONE; etype < FEEDBACK_TYPE_END; ++etype) {
		if (!strncmp(type, str_type[etype], strlen(type)))
			break;
	}

	if (etype == FEEDBACK_TYPE_END) {
		_E("Invalid parameter : type(%s)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	for (epattern = 0; epattern < FEEDBACK_PATTERN_END; ++epattern) {
		if (!strncmp(pattern, str_pattern[epattern], strlen(pattern)))
			break;
	}

	if (epattern == FEEDBACK_PATTERN_END) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	return feedback_play_type(etype, epattern);
}

API int feedback_stop(void)
{
	/* check initialize */
	if (!binit) {
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	/* stop all device */
	devices_stop();

	return FEEDBACK_ERROR_NONE;
}

API int feedback_get_resource_path(feedback_type_e type, feedback_pattern_e pattern, char** path)
{
	const struct device_ops *dev;
	char buf[PATH_MAX] = {0,};
	int err;

	if (path == NULL) {
		_E("Invalid parameter : path(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		_E("Invalid parameter : pattern(%d)", pattern);
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

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		_E("Invalid parameter : pattern(%d)", pattern);
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
