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

#ifndef __PARSER_H__
#define __PARSER_H__

struct feedback_data {
	char *origin;
	char *changed;
};

struct feedback_config_info {
	char *name;
	struct feedback_data *data;
};

int feedback_load_config(const char *path,
		struct feedback_config_info *info);
void feedback_free_config(struct feedback_config_info *info);

#endif
