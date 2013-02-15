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

#define MAX_PATH_LENGTH      256
#define NOT_ASSIGNED         NULL

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

static feedback_h _feedback_handle = NOT_ASSIGNED;

API int feedback_initialize()
{
	int err = -1;

	if (_feedback_handle != NOT_ASSIGNED) {
		FEEDBACK_LOG("Already initialized");
		return FEEDBACK_ERROR_NONE;
	}

	err = _feedback_init(&_feedback_handle);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("_feedback_init is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_deinitialize()
{
	int err = -1;

	if (_feedback_handle == NOT_ASSIGNED) {
		FEEDBACK_ERROR("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	err = _feedback_fini(_feedback_handle);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("_feedback_fini is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	_feedback_handle = NOT_ASSIGNED;
	return FEEDBACK_ERROR_NONE;
}

API int feedback_play(feedback_pattern_e pattern)
{
	int err = -1;

	if (_feedback_handle == NOT_ASSIGNED) {
		FEEDBACK_ERROR("Not initialized");
		return FEEDBACK_ERROR_NOT_INITIALIZED;
	}

	if (pattern < FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	err = _feedback_play_sound(_feedback_handle, pattern);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("_feedback_play_sound is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	err = _feedback_play_vibration(_feedback_handle, pattern);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("_feedback_play_vibration is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_play_type(feedback_type_e type, feedback_pattern_e pattern)
{
	int err = -1;

	if (_feedback_handle == NOT_ASSIGNED) {
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

	switch(type) {
		case FEEDBACK_TYPE_SOUND:
			err = _feedback_play_sound(_feedback_handle, pattern);
			break;
		case FEEDBACK_TYPE_VIBRATION:
			err = _feedback_play_vibration(_feedback_handle, pattern);
			break;
		default:
			FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
	        return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("_feedback_play(type:%d) is failed", type);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

API int feedback_get_resource_path(feedback_type_e type, feedback_pattern_e pattern, char** path)
{
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

	err = _feedback_get_path(type, pattern, buf, MAX_PATH_LENGTH);
	if (FEEDBACK_FAILED(err)) {
		FEEDBACK_ERROR("_feedback_get_path is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	*path = strdup(buf);

	return FEEDBACK_ERROR_NONE;
}
