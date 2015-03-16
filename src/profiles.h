/*
 * libfeedback

 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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


#ifndef __PROFILES_H__
#define __PROFILES_H__

#include <stdbool.h>
#include "common.h"

struct profile_ops {
	const char *profile;
	void (*init) (void);
	void (*exit) (void);
	bool (*get_switched_pattern)(int pattern, int *switched);
	bool (*get_always_alert_case)(int type, int pattern);
	bool (*get_always_off_case)(int type, int pattern);
	int (*get_strength_type)(int type, int pattern);
	const int max_type;
	const int max_pattern;
	const char **str_type;
	const char **str_pattern;
};

#define PROFILE_OPS_REGISTER(ops)	\
	const struct profile_ops *profile = ops

extern const struct profile_ops *profile;

#define CHECK_VIBRATION(type)   (type == FEEDBACK_TYPE_VIBRATION)
#define CHECK_SOUND(type)       (type == FEEDBACK_TYPE_SOUND)

#endif
