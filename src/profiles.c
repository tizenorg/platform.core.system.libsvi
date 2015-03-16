/*
 * deviced
 *
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


#include <stdio.h>
#include <limits.h>

#include "profiles.h"
#include "log.h"

static dd_list *profile_head;

void add_profile(const struct profile_ops *ops)
{
	DD_LIST_APPEND(profile_head, (struct profile_ops*)ops);
}

void remove_profile(const struct profile_ops *ops)
{
	DD_LIST_REMOVE(profile_head, (struct profile_ops*)ops);
}
