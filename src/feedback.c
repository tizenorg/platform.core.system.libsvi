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

#include "feedback.h"
#include "profiles.h"
#include "devices.h"
#include "log.h"

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

static bool binit;

API int feedback_initialize(void)
{
	if (binit)
		return FEEDBACK_ERROR_NONE;

	if (!profile) {
		_E("there is no valid profile module.");
		return FEEDBACK_ERROR_NOT_SUPPORTED;
	}

	/* initialize device */
	devices_init();

	/* initialize profile feature */
	if (profile->init)
		profile->init();

	binit = true;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_deinitialize(void)
{
	if (!binit)
		return FEEDBACK_ERROR_NOT_INITIALIZED;

	/* deinitialize device */
	devices_exit();

	/* deinitialize profile feature */
	if (profile->exit)
		profile->exit();

	binit = false;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_play(feedback_pattern_e pattern)
{
	int err;
	bool result;
	int switched;

	/* check initialize */
	if (!binit) {
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	/* if you need to switch pattern */
	if (profile->get_switched_pattern) {
		result = profile->get_switched_pattern(pattern, &switched);
		if (result) {
			_W("pattern is changed : (%s) -> (%s)",
					profile->str_pattern[pattern], profile->str_pattern[switched]);
			pattern = switched;
		}
	}

	/* play all device */
	err = devices_play(pattern);
	/**
	 * devices_play() returns error even if all devices are failed.
	 * It means if to play anything is successful,
	 * this function regards as success.
	 */
	if (err == -ENOTSUP)
		return FEEDBACK_ERROR_NOT_SUPPORTED;
	else if (err < 0)
		return FEEDBACK_ERROR_OPERATION_FAILED;

	return FEEDBACK_ERROR_NONE;
}

API int feedback_play_type(feedback_type_e type, feedback_pattern_e pattern)
{
	const struct device_ops *dev;
	int err;
	bool result;
	int switched;

	/* check initialize */
	if (!binit) {
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (type <= FEEDBACK_TYPE_NONE ||
	    type >= profile->max_type) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	/* if you need to switch pattern */
	if (profile->get_switched_pattern) {
		result = profile->get_switched_pattern(pattern, &switched);
		if (result) {
			_W("pattern is changed : (%s) -> (%s)",
					profile->str_pattern[pattern], profile->str_pattern[switched]);
			pattern = switched;
		}
	}

	/* play proper device */
	dev = find_device(type);
	if (!dev) {
		_E("Not supported device : type(%s)", profile->str_type[type]);
		return FEEDBACK_ERROR_NOT_SUPPORTED;
	}

	err = dev->play(pattern);
	if (err == -ENOTSUP)
		return FEEDBACK_ERROR_NOT_SUPPORTED;
	else if (err == -ECOMM)
		return FEEDBACK_ERROR_PERMISSION_DENIED;
	else
		return FEEDBACK_ERROR_OPERATION_FAILED;

	return FEEDBACK_ERROR_NONE;
}

API int feedback_stop(void)
{
	int err;

	/* check initialize */
	if (!binit) {
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	/* stop all device */
	err = devices_stop();
	if (err == -ENOTSUP)
		return FEEDBACK_ERROR_NOT_SUPPORTED;
	else if (err == -ECOMM)
		return FEEDBACK_ERROR_PERMISSION_DENIED;
	else if (err < 0)
		return FEEDBACK_ERROR_OPERATION_FAILED;

	return FEEDBACK_ERROR_NONE;
}

API int feedback_is_supported_pattern(feedback_type_e type, feedback_pattern_e pattern, bool *status)
{
	const struct device_ops *dev;
	bool supported;
	int err;
	bool result;
	int switched;

	/* check initialize */
	if (!binit) {
		_E("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (!status) {
		_E("Invalid parameter : status(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type <= FEEDBACK_TYPE_NONE ||
	    type >= profile->max_type) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	/* if you need to switch pattern */
	if (profile->get_switched_pattern) {
		result = profile->get_switched_pattern(pattern, &switched);
		if (result) {
			_W("pattern is changed : (%s) -> (%s)",
					profile->str_pattern[pattern], profile->str_pattern[switched]);
			pattern = switched;
		}
	}

	/* play proper device */
	dev = find_device(type);
	if (!dev) {
		_E("Not supported device : type(%s)", profile->str_type[type]);
		return FEEDBACK_ERROR_NOT_SUPPORTED;
	}

	err = dev->is_supported(pattern, &supported);
	if (err < 0) {
		_E("fail to invoke is_supported() : pattern(%s)", profile->str_pattern[pattern]);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

/* Internal APIs */
API int feedback_play_type_by_name(char *type, char *pattern)
{
	feedback_type_e etype;
	feedback_pattern_e epattern;
	int type_max;
	int pattern_max;

	if (!type || !pattern) {
		_E("Invalid parameter : type(%x), pattern(%x)", type, pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	type_max = profile->max_type;
	for (etype = 0; etype < type_max; ++etype) {
		if (!strncmp(type, profile->str_type[etype], strlen(type)))
			break;
	}

	if (etype == type_max) {
		_E("Invalid parameter : type(%s)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	pattern_max = profile->max_pattern;
	for (epattern = 0; epattern < pattern_max; ++epattern) {
		if (!strncmp(pattern, profile->str_pattern[epattern], strlen(pattern)))
			break;
	}

	if (epattern == pattern_max) {
		_E("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	return feedback_play_type(etype, epattern);
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

	if (type <= FEEDBACK_TYPE_NONE ||
	    type >= profile->max_type) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
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

	if (type <= FEEDBACK_TYPE_NONE ||
	    type >= profile->max_type) {
		_E("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern) {
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
