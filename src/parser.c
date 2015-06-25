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

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "parser.h"
#include "profiles.h"
#include "log.h"

#define MAX_DATA	256
#define MAX_LINE	256
#define MAX_SECTION	64
#define WHITESPACE	" \t"
#define NEWLINE		"\n\r"
#define COMMENT		'#'

#define MATCH(a, b)             (!strncmp(a, b, strlen(a)))
#define SET_CONF(a, b)          (a = (b > 0.0 ? b : a))

struct parse_result {
	char *section;
	char *name;
	char *value;
};

static inline char *trim_str(char *s)
{
	char *t;
	/* left trim */
	s += strspn(s, WHITESPACE);

	/* right trim */
	for (t = strchr(s, 0); t > s; t--)
		if (!strchr(WHITESPACE, t[-1]))
			break;
	*t = 0;
	return s;
}

static int config_parse(const char *file_name,
		int cb(struct parse_result *result, void *user_data),
		void *user_data)
{
	FILE *f = NULL;
	struct parse_result result;
	/* use stack for parsing */
	char line[MAX_LINE];
	char section[MAX_SECTION];
	char *start, *end, *name, *value;
	int lineno = 0, ret = 0;

	if (!file_name || !cb) {
		ret = -EINVAL;
		goto error;
	}

	/* open conf file */
	f = fopen(file_name, "r");
	if (!f) {
		_E("Failed to open file %s", file_name);
		ret = -EIO;
		goto error;
	}

	/* parsing line by line */
	while (fgets(line, MAX_LINE, f) != NULL) {
		lineno++;

		start = line;
		start[strcspn(start, NEWLINE)] = '\0';
		start = trim_str(start);

		if (*start == COMMENT) {
			continue;
		} else if (*start == '[') {
			/* parse section */
			end = strchr(start, ']');
			if (!end || *end != ']') {
				ret = -EBADMSG;
				goto error;
			}

			*end = '\0';
			strncpy(section, start + 1, sizeof(section));
			section[MAX_SECTION-1] = '\0';
		} else if (*start) {
			/* parse name & value */
			end = strchr(start, '=');
			if (!end || *end != '=') {
				ret = -EBADMSG;
				goto error;
			}
			*end = '\0';
			name = trim_str(start);
			value = trim_str(end + 1);
			end = strchr(value, COMMENT);
			if (end && *end == COMMENT) {
				*end = '\0';
				value = trim_str(value);
			}

			result.section = section;
			result.name = name;
			result.value = value;
			/* callback with parse result */
			ret = cb(&result, user_data);
			if (ret < 0) {
				ret = -EBADMSG;
				goto error;
			}
		}
	}
	_D("Success to load %s", file_name);
	fclose(f);
	return 0;

error:
	if (f)
		fclose(f);
	_E("Failed to read %s:%d!", file_name, lineno);
	return ret;
}

static int load_config(struct parse_result *result, void *user_data)
{
	struct feedback_config_info *info = (struct feedback_config_info *)user_data;
	char *name;
	char *value;
	int i;

	if (!info)
		return -EINVAL;

	if (!MATCH(result->section, info->name))
		return -ENOENT;

	if (!result->name || !result->value)
		return -ENOENT;

	name = result->name;
	value = result->value;

	for (i = 0; i < profile->max_pattern; i++) {
		if (!MATCH(name, (char *)profile->str_pattern[i]))
			continue;

		info->data[i].origin = strdup(value);
		if (!info->data[i].origin)
			_E("fail to copy %s sound data",
					profile->str_pattern[i]);
		break;
	}

	return 0;
}

int feedback_load_config(const char *path,
		struct feedback_config_info *info)
{
	int ret;

	if (!path || !info)
		return -EINVAL;

	info->data = calloc(1,
			sizeof(struct feedback_data) * profile->max_pattern);
	if (!info->data) {
		_E("fail to allocate %s data", path);
		return -ENOMEM;
	}

	ret = config_parse(path, load_config, info);
	if (ret < 0)
		_E("Failed to load %s, %d Use default value!",
				path, ret);

	return ret;
}

void feedback_free_config(struct feedback_config_info *info)
{
	int i;

	if (!info || !(info->data))
		return;

	if (!profile)
		return;

	for (i = 0; i < profile->max_pattern; i++) {
		if (info->data[i].origin) {
			free(info->data[i].origin);
			info->data[i].origin = NULL;
		}
		if (info->data[i].changed) {
			free(info->data[i].changed);
			info->data[i].changed = NULL;
		}
	}

	free(info->data);
}
