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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <vconf.h>
#include <mm_sound_private.h>

#include "feedback-ids.h"
#include "profiles.h"
#include "devices.h"
#include "log.h"
#include "parser.h"

#define SOUND_CONF_FILE "/usr/share/feedback/sound.conf"

/* Temporary keys */
#ifndef VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL
#define VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL "db/setting/sound/button_sounds"
#endif

static int sndstatus;
static int touch_sndstatus;
static int keytone_sndstatus;
static struct feedback_config_info sound_info = {
	.name = "Sound",
};

static char *get_data(feedback_pattern_e pattern)
{
	char *data;

	if (pattern <= FEEDBACK_PATTERN_NONE ||
	    pattern >= profile->max_pattern)
		return NULL;

	if (sound_info.data[pattern].changed)
		data = sound_info.data[pattern].changed;
	else
		data = sound_info.data[pattern].origin;

	return data;
}

inline int is_sound_mode(void)
{
	return sndstatus;
}

inline int is_touch_sndstatus(void)
{
	return touch_sndstatus;
}

inline int is_keytone_sndstatus(void)
{
	return keytone_sndstatus;
}

static void feedback_touch_sndstatus_cb(keynode_t *key, void* data)
{
	touch_sndstatus = vconf_keynode_get_bool(key);
}

static void feedback_keytone_sndstatus_cb(keynode_t *key, void* data)
{
	keytone_sndstatus = vconf_keynode_get_bool(key);
}

static void sound_init(void)
{
	/* get sound data */
	feedback_load_config(SOUND_CONF_FILE, &sound_info);

	/* check sound status */
	if (vconf_get_bool(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, &touch_sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL ==> FAIL!!");

	if (vconf_get_bool(VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL, &keytone_sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL ==> FAIL!!");

	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) < 0) {
		_D("fail to get sound status, will work as turning off");
		sndstatus = 0;
	}

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL, feedback_keytone_sndstatus_cb, NULL);
}

static void sound_exit(void)
{
	/* remove watch */
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_BUTTON_SOUNDS_BOOL, feedback_keytone_sndstatus_cb);

	/* free sound data */
	feedback_free_config(&sound_info);
}

static int sound_play(feedback_pattern_e pattern)
{
	struct stat buf;
	int retry = FEEDBACK_RETRY_CNT, ret;
	char *path;
	int level;

	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) < 0) {
		_D("fail to get sound status, will work as turning off");
		sndstatus = 0;
	}

	if (sndstatus == 0 && profile->get_always_alert_case &&
	    !profile->get_always_alert_case(FEEDBACK_TYPE_SOUND, pattern)) {
		_D("Sound condition is OFF (sndstatus : %d)", sndstatus);
		return 0;
	}

	if (sndstatus && profile->get_always_off_case &&
	    profile->get_always_off_case(FEEDBACK_TYPE_SOUND, pattern)) {
		_D("Sound always off condition");
		return 0;
	}

	/* get sound file path */
	path = get_data(pattern);
	if (!path || stat(path, &buf)) {
		_E("Not supported sound pattern");
		return -ENOTSUP;
	}

	/* play sound file */
	do {
		if (profile->get_strength_type)
			level = profile->get_strength_type(FEEDBACK_TYPE_SOUND, pattern);
		else
			level = VOLUME_TYPE_SYSTEM;

		ret = mm_sound_play_keysound(path, level);
		if (ret == MM_ERROR_NONE) {
			_D("Play success! SND filename is %s", path);
			return 0;
		}
		_E("mm_sound_play_keysound() returned error(%d)", ret);
	} while (retry--);

	return -EPERM;
}

static int sound_stop()
{
	return 0;
}

static int sound_is_supported(feedback_pattern_e pattern, bool *supported)
{
	struct stat buf;
	char *path;
	bool ret = true;

	if (!supported) {
		_E("Invalid parameter : supported(NULL)");
		return -EINVAL;
	}

	/* get sound file path */
	path = get_data(pattern);
	if (!path || stat(path, &buf)) {
		_E("%s is not presents", path);
		ret = false;
	}

	*supported = ret;
	return 0;
}

static int sound_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	char *cur_path;
	int ret = 0;

	if (!buf || buflen <= 0)
		return -EINVAL;

	/* get sound file path */
	cur_path = get_data(pattern);
	if (!cur_path) {
		_E("This pattern(%s) in sound type is not supported to play",
				profile->str_pattern[pattern]);
		cur_path = "NULL";
		ret = -ENOENT;
	}

	snprintf(buf, buflen, "%s", cur_path);
	return ret;
}

static int sound_set_path(feedback_pattern_e pattern, char *path)
{
	struct stat buf;

	/*
	 * check the path is valid
	 * if path is null, below operation is ignored
	 */
	if (path && stat(path, &buf)) {
		_E("%s is not presents", path);
		return -errno;
	}

	if (sound_info.data[pattern].changed) {
		free(sound_info.data[pattern].changed);
		sound_info.data[pattern].changed = NULL;
	}

	/* if path is NULL, this pattern set to default file */
	if (path)
		sound_info.data[pattern].changed = strdup(path);

	_D("The file of pattern(%s) is changed to [%s]",
			profile->str_pattern[pattern], path);
	return 0;
}

static const struct device_ops sound_device_ops = {
	.type = FEEDBACK_TYPE_SOUND,
	.name = "Sound",
	.init = sound_init,
	.exit = sound_exit,
	.play = sound_play,
	.stop = sound_stop,
	.is_supported = sound_is_supported,
	.get_path = sound_get_path,
	.set_path = sound_set_path,
};

DEVICE_OPS_REGISTER(&sound_device_ops);
