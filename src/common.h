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

#include <stdbool.h>
#include <mm_sound_private.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file common.h
 * @brief This file contains the common information
 */

#ifndef __CONSTRUCTOR__
#define __CONSTRUCTOR__ __attribute__ ((constructor))
#endif

#ifndef __DESTRUCTOR__
#define __DESTRUCTOR__ __attribute__ ((destructor))
#endif

#ifndef __DD_LIST__
#define __DD_LIST__
#include <glib.h>
typedef GList dd_list;
#define DD_LIST_PREPEND(a, b)       \
	a = g_list_prepend(a, b)
#define DD_LIST_APPEND(a, b)        \
	a = g_list_append(a, b)
#define DD_LIST_REMOVE(a, b)        \
	a = g_list_remove(a, b)
#define DD_LIST_FOREACH(head, elem, node)   \
	for (elem = head; elem && ((node = elem->data) != NULL); elem = elem->next, node = NULL)
#endif

#define FEEDBACK_DATA_DIR			"/opt/usr/share/feedback"
#define FEEDBACK_ORIGIN_DATA_DIR	"/usr/share/feedback"

#define FEEDBACK_RETRY_CNT       1

#define DEFAULT_VIB_LEVEL       3
#define HAPTIC_FEEDBACK_STEP    20 /**< feedback max / slider step */

int is_sound_mode(void);
int is_touch_sndstatus(void);
int is_keytone_sndstatus(void);
int is_vibration_mode(void);

#ifdef __cplusplus
}
#endif

#endif
