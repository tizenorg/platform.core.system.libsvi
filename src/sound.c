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
#include "common.h"
#include "log.h"
#include "devices.h"
#include "xmlparser.h"

#define SOUND_XML					"/usr/share/feedback/sound.xml"

static int sndstatus;
static int touch_sndstatus;
static int lock_sndstatus;
static int camerastatus;
static int shutter_sndstatus;

static xmlDocPtr v_doc;

static char sound_file[FEEDBACK_PATTERN_END][NAME_MAX];

inline int is_sound_mode(void)
{
	return sndstatus;
}

static void feedback_touch_sndstatus_cb(keynode_t *key, void* data)
{
	touch_sndstatus = vconf_keynode_get_bool(key);
}

static void feedback_lock_sndstatus_cb(keynode_t *key, void* data)
{
	lock_sndstatus = vconf_keynode_get_bool(key);
}

static void feedback_camerastatus_cb(keynode_t *key, void* data)
{
	camerastatus = vconf_keynode_get_int(key);
}

static volume_type_t get_volume_type(feedback_pattern_e pattern)
{
	if (pattern == FEEDBACK_PATTERN_TAP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_TOUCH;
	else if (pattern >= FEEDBACK_PATTERN_KEY0 && pattern <= FEEDBACK_PATTERN_KEY_BACK)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_DIALER;
	else if (pattern == FEEDBACK_PATTERN_VOLUME_KEY)
		return VOLUME_TYPE_RINGTONE;
	else if (camerastatus && shutter_sndstatus && pattern == FEEDBACK_PATTERN_SCREEN_CAPTURE)
		return VOLUME_TYPE_FIXED;

	return VOLUME_TYPE_SYSTEM;
}

static bool get_always_alert_case(feedback_pattern_e pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_WAKEUP_ON_CALL:
		return true;
	case FEEDBACK_PATTERN_MESSAGE_ON_CALL:
	case FEEDBACK_PATTERN_EMAIL_ON_CALL:
	case FEEDBACK_PATTERN_GENERAL_ON_CALL:
		if (alert_callstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_SMART_ALERT:
	case FEEDBACK_PATTERN_SEND_SOS_MESSAGE:
	case FEEDBACK_PATTERN_END_SOS_MESSAGE:
	case FEEDBACK_PATTERN_CMAS:
		return true;
	case FEEDBACK_PATTERN_SCREEN_CAPTURE:
		if (camerastatus && shutter_sndstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_OUTGOING_CALL:
		return true;
	default:
		break;
	}
	return false;
}

static bool get_always_off_case(feedback_pattern_e pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_TAP ... FEEDBACK_PATTERN_MAX_CHARACTER:
	case FEEDBACK_PATTERN_HOLD ... FEEDBACK_PATTERN_HW_HOLD:
		if (!touch_sndstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_KEY0 ... FEEDBACK_PATTERN_KEY_BACK:
		break;
	case FEEDBACK_PATTERN_LOCK:
	case FEEDBACK_PATTERN_UNLOCK:
	case FEEDBACK_PATTERN_LOCK_SWIPE:
	case FEEDBACK_PATTERN_UNLOCK_SWIPE:
		if (!lock_sndstatus)
			return true;
		break;
	default:
		break;
	}
	return false;
}

static int get_xml_data(xmlDocPtr doc, feedback_pattern_e pattern, struct xmlData **data)
{
	xmlNodePtr cur;
	struct xmlData *retData;

	cur = xml_find(doc, SOUND_STR, (const xmlChar*)str_pattern[pattern]);
	/* This pattern does not have sound file to play */
	if (cur == NULL)
		return -ENOENT;

	retData = xml_parse(doc, cur);
	if (retData == NULL) {
		_E("xml_parse fail");
		return -EPERM;
	}

	*data = retData;
	return 0;
}

static void release_xml_data(struct xmlData *data)
{
	if (data == NULL)
		return;

	xml_free(data);
}

static void sound_init(void)
{
	/* xml Init */
	v_doc = xml_open(SOUND_XML);
	if (v_doc == NULL) {
		_E("xml_open(%s) fail", SOUND_XML);
		return;
	}

	/* check sound status */
	if (vconf_get_bool(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, &touch_sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL ==> FAIL!!");

	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_LOCK_BOOL, &lock_sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_SOUND_LOCK_BOOL ==> FAIL!!");

	/* check camera status */
	if (vconf_get_int(VCONFKEY_CAMERA_STATE, &camerastatus) < 0)
		_W("VCONFKEY_CAMERA_STATE ==> FAIL!!");

	/* shutter sound policy */
	// This vconf is read just once, because this value is not changed in running time.
	if (vconf_get_int(VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY, &shutter_sndstatus) < 0)
		_W("VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_LOCK_BOOL, feedback_lock_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_CAMERA_STATE, feedback_camerastatus_cb, NULL);
}

static void sound_exit(void)
{
	/* remove watch */
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_LOCK_BOOL, feedback_lock_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_CAMERA_STATE, feedback_camerastatus_cb);

	if (v_doc) {
		xml_close(v_doc);
		v_doc = NULL;
	}
}

static int sound_play(feedback_pattern_e pattern)
{
	struct stat buf;
	int retry = FEEDBACK_RETRY_CNT, ret;
	char *path;
	struct xmlData *data = NULL;

	if (!v_doc) {
		_E("Not initialize");
		return -EPERM;
	}

	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) < 0) {
		_D("fail to get sound status, will work as turning off");
		sndstatus = 0;
	}

	if (sndstatus == 0 && !get_always_alert_case(pattern)) {
		_D("Sound condition is OFF (sndstatus : %d)", sndstatus);
		return 0;
	}

	if (sndstatus && get_always_off_case(pattern)) {
		_D("Sound always off condition");
		return 0;
	}

	/* check if the state of voice recorder is recording */
	if (vconf_get_int(VCONFKEY_SOUND_STATUS, &ret) < 0) {
		_D("fail to get media sound status, status will be zero");
		ret = 0;
	}

	if (ret & VCONFKEY_SOUND_STATUS_AVRECORDING) {
		_D("voice recording status is RECORDING");
		return 0;
	}

	/* check whether there is a user defined file */
	path = sound_file[pattern];

	/* if not */
	if (!(*path)) {
		ret = get_xml_data(v_doc, pattern, &data);
		if (ret == -ENOENT) {
			_D("No sound case(%s)", str_pattern[pattern]);
			return 0;
		}

		if (ret < 0) {
			_E("get_xml_data fail");
			return -EPERM;
		}

		if (!data->data) {
			_D("No sound case(%s)", str_pattern[pattern]);
			release_xml_data(data);
			return 0;
		}

		path = data->data;
	}

	if (stat(path, &buf)) {
		_E("%s is not presents", path);
		return -ENOENT;
	}

	/* play sound file */
	do {
		ret = mm_sound_play_keysound(path, get_volume_type(pattern));
		if (ret == MM_ERROR_NONE) {
			_D("Play success! SND filename is %s", path);
			release_xml_data(data);
			return 0;
		}
		_E("mm_sound_play_keysound() returned error(%d)", ret);
	} while(retry--);

	release_xml_data(data);
	return -EPERM;
}

static int sound_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	char *cur_path;
	int ret = 0;
	struct xmlData *data = NULL;

	if (!buf || buflen <= 0)
		return -EINVAL;

	cur_path = sound_file[pattern];
	if (!cur_path) {
		ret = get_xml_data(v_doc, pattern, &data);
		if (ret >= 0 && data && data->data)
			cur_path = (char*)data->data;
	}

	if (!cur_path) {
		_E("This pattern(%s) in sound type is not supported to play", str_pattern[pattern]);
		cur_path = "NULL";
		ret = -ENOENT;
	}

	snprintf(buf, buflen, "%s", cur_path);
	release_xml_data(data);
	return 0;
}

static int sound_set_path(feedback_pattern_e pattern, char *path)
{
	struct stat buf;
	char *ppath;

	/*
	 * check the path is valid
	 * if path is null, below operation is ignored
	 */
	if (path && stat(path, &buf)) {
		_E("%s is not presents", path);
		return -errno;
	}

	ppath = sound_file[pattern];

	/* if path is NULL, this pattern set to default file */
	if (path)
		snprintf(ppath, NAME_MAX, "%s", path);
	else
		memset(ppath, 0, NAME_MAX);

	_D("The file of pattern(%s) is changed to [%s]", str_pattern[pattern], path);
	return 0;
}

static const struct device_ops sound_device_ops = {
	.type = FEEDBACK_TYPE_SOUND,
	.init = sound_init,
	.exit = sound_exit,
	.play = sound_play,
	.get_path = sound_get_path,
	.set_path = sound_set_path,
};

DEVICE_OPS_REGISTER(&sound_device_ops);
