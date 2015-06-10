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


#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file common.h
 * @brief This file contains the common information
 */

#define FEEDBACK_DATA_DIR			"/opt/usr/share/feedback"
#define FEEDBACK_ORIGIN_DATA_DIR	"/usr/share/feedback"

#define FEEDBACK_RETRY_CNT       1

extern int callstatus;
extern int alert_callstatus;

extern const char *str_type[];
extern const char *str_pattern[];

int is_sound_mode(void);

#ifdef __cplusplus
}
#endif

#endif
