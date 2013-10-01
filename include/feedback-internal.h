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


#ifndef __FEEDBACK_LEGACY_H__
#define __FEEDBACK_LEGACY_H__

#include "feedback-ids.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file feedback-legacy.h
 * @brief This file contains the feedback legacy API
 */

#define FEEDBACK_DATA_DIR			"/opt/usr/share/feedback"
#define FEEDBACK_ORIGIN_DATA_DIR	"/usr/share/feedback"

#define FEEDBACK_RETRY_CNT       1
#define MAX_FILE_PATH          512

typedef void* feedback_h;

int feedback_init(feedback_h *handle);
int feedback_fini(feedback_h handle);
int feedback_play_sound(feedback_h handle, feedback_pattern_e key);
int feedback_play_vibration(feedback_h handle, feedback_pattern_e key);
int feedback_set_path(feedback_type_e type, feedback_pattern_e key, char* path);
int feedback_get_path(feedback_type_e type, feedback_pattern_e key, char* buf, unsigned int buflen);

int callstatus;

feedback_pattern_e feedback_get_alert_on_call_key(feedback_pattern_e pattern);

#ifdef __cplusplus
}
#endif

#endif //__SVI_H__
