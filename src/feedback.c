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
#include <string.h>
#include "feedback.h"
#include "feedback-internal.h"
#include "feedback-log.h"
#include "devices.h"

#define MAX_PATH_LENGTH      256
#define NOT_ASSIGNED         NULL

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

static feedback_h feedback_handle = NOT_ASSIGNED;

API int feedback_initialize()
{
	int err = -1;

	if (feedback_handle != NOT_ASSIGNED) {
		FEEDBACK_LOG("Already initialized");
		return FEEDBACK_ERROR_NONE;
	}

	/* initialize device */
	devices_init();

	err = feedback_init(&feedback_handle);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("feedback_init is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_deinitialize()
{
	int err = -1;

	if (feedback_handle == NOT_ASSIGNED) {
		FEEDBACK_ERROR("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	/* deinitialize device */
	devices_exit();

	err = feedback_fini(feedback_handle);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("feedback_fini is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	feedback_handle = NOT_ASSIGNED;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_play(feedback_pattern_e pattern)
{
	int err = -1;

	if (feedback_handle == NOT_ASSIGNED) {
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

	/* play all device */
	devices_play(pattern);

	err = feedback_play_vibration(feedback_handle, pattern);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("feedback_play_vibration is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_play_type(feedback_type_e type, feedback_pattern_e pattern)
{
	const struct device_ops *dev;
	int err = -1;

	if (feedback_handle == NOT_ASSIGNED) {
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

	switch(type) {
		case FEEDBACK_TYPE_SOUND:
			dev = find_device(type);
			if (dev) {
				err = dev->play(pattern);
				if (err < 0)
					FEEDBACK_ERROR("feedback_play_sound is failed");
			}
			break;
		case FEEDBACK_TYPE_VIBRATION:
			err = feedback_play_vibration(feedback_handle, pattern);
			if (FEEDBACK_FAILED(err))
				FEEDBACK_ERROR("feedback_play(type:%d) is failed", type);
			break;
		default:
			FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
	        return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_get_resource_path(feedback_type_e type, feedback_pattern_e pattern, char** path)
{
	const struct device_ops *dev;
	int err = -1;
	char buf[MAX_PATH_LENGTH] = {0,};

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

	if (type == FEEDBACK_TYPE_SOUND) {
		dev = find_device(type);
		if (dev)
			err = dev->get_path(pattern, buf, sizeof(buf));
	} else if (type == FEEDBACK_TYPE_VIBRATION)
		err = feedback_get_path(type, pattern, buf, MAX_PATH_LENGTH);

	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("feedback_get_path is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	*path = strdup(buf);
	return FEEDBACK_ERROR_NONE;
}

API int feedback_set_resource_path(feedback_type_e type, feedback_pattern_e pattern, char* path)
{
	const struct device_ops *dev;
	int err = -1;

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

	if (type == FEEDBACK_TYPE_SOUND) {
		dev = find_device(type);
		if (dev)
			err = dev->set_path(pattern, path);
	} else if (type == FEEDBACK_TYPE_VIBRATION)
		err = feedback_set_path(type, pattern, path);

	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("feedback_set_path is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}
