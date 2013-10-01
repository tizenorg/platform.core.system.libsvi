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
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mm_sound_private.h>
#include <vconf.h>
#include <haptic.h>
#include <fcntl.h>
#include <errno.h>

#include "feedback.h"
#include "feedback-internal.h"
#include "feedback-file.h"
#include "feedback-str.h"
#include "feedback-log.h"
#include "xmlparser.h"

#define FEEDBACK_RETRY_CNT       1
#define MAX_FILE_PATH          512

#define DEFAULT_FEEDBACK_HANDLE		0x0F

#define VIBRATION_XML				"/usr/share/feedback/vibration.xml"

typedef struct {
	haptic_device_h v_handle;
	xmlDocPtr v_doc;
} FEEDBACK_HANDLE;

static int sndstatus;
static int touch_sndstatus;
static int soundon;
static int vibstatus;
static int vib_level;
static int noti_level;
static int callstatus;

static void feedback_sndstatus_cb(keynode_t *key, void* data)
{
	sndstatus = vconf_keynode_get_bool(key);
}

static void feedback_touch_sndstatus_cb(keynode_t *key, void* data)
{
	touch_sndstatus = vconf_keynode_get_bool(key);
}

static void feedback_soundon_cb(keynode_t *key, void* data)
{
	soundon = vconf_keynode_get_int(key);
}

static void feedback_vibstatus_cb(keynode_t *key, void* data)
{
	vibstatus = vconf_keynode_get_bool(key);
}

static void feedback_vib_cb(keynode_t *key, void* data)
{
	vib_level = vconf_keynode_get_int(key);
}

static void feedback_noti_cb(keynode_t *key, void* data)
{
	noti_level = vconf_keynode_get_int(key);
}


static void feedback_callstatus_cb(keynode_t *key, void* data)
{
	callstatus = vconf_keynode_get_int(key);
}

static feedback_pattern_e feedback_get_alert_on_call_key(feedback_pattern_e pattern)
{
	switch(pattern) {
	case FEEDBACK_PATTERN_MESSAGE:
	case FEEDBACK_PATTERN_EMAIL:
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_SCHEDULE:
	case FEEDBACK_PATTERN_TIMER:
	case FEEDBACK_PATTERN_GENERAL:
	case FEEDBACK_PATTERN_CHARGERCONN:
	case FEEDBACK_PATTERN_FULLCHARGED:
	case FEEDBACK_PATTERN_LOWBATT:
		return (feedback_pattern_e)(pattern+1);
	default:
		break;
	}

	return pattern;
}

static haptic_priority_e feedback_get_priority(feedback_pattern_e pattern)
{
	if (pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD)
		return HAPTIC_PRIORITY_MIN;

	return HAPTIC_PRIORITY_MIDDLE;
}

static volume_type_t feedback_get_volume_type(feedback_pattern_e pattern)
{
	if (pattern == FEEDBACK_PATTERN_TAP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_TOUCH;
	else if (pattern >= FEEDBACK_PATTERN_KEY0 && pattern <= FEEDBACK_PATTERN_KEY_SHARP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_DIALER;
	else if (pattern == FEEDBACK_PATTERN_VOLUME_KEY)
		return VOLUME_TYPE_RINGTONE;

	return VOLUME_TYPE_SYSTEM;
}

static int feedback_get_haptic_level(feedback_pattern_e pattern)
{
	int level = -1;

	if (pattern >= FEEDBACK_PATTERN_MESSAGE && pattern <= FEEDBACK_PATTERN_GENERAL_ON_CALL)
		level = noti_level;
	else
		level = vib_level;

	FEEDBACK_LOG("Call status : %d, pattern : %s, level : %d", callstatus, str_pattern[pattern], level);
	if (callstatus != VCONFKEY_CALL_OFF) {
		// if call status is ON, vibration magnitude is 20%
		level = (int)(level*0.2f);
		level = (level < 1) ? 1 : level;
		FEEDBACK_LOG("level changed : %d", level);
	}

	level = level * 20;
	return level;
}

static bool feedback_get_always_alert_case(feedback_pattern_e pattern)
{
	switch(pattern) {
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_WAKEUP_ON_CALL:
		return true;
	default:
		break;
	}
	return false;
}

static int feedback_get_data(xmlDocPtr doc, feedback_pattern_e pattern, struct xmlData **data)
{
	xmlNodePtr cur;
	struct xmlData *retData;

	cur = xml_find(doc, (const xmlChar*)str_pattern[pattern]);
	if (cur == NULL) {
		FEEDBACK_ERROR("xml_find fail");
		return -1;
	}

	retData = xml_parse(doc, cur);
	if (retData == NULL) {
		FEEDBACK_ERROR("xml_parse fail");
		return -1;
	}

	*data = retData;
	return 0;
}

static void feedback_release_data(struct xmlData *data)
{
	if (data == NULL)
		return;

	xml_free(data);
}

static int feedback_change_symlink(const char *sym_path, const char *new_path)
{
	struct stat buf;

	if (sym_path == NULL || strlen(sym_path) == 0) {
		FEEDBACK_ERROR("Invalid parameter : sym_path(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (new_path == NULL || strlen(new_path) == 0) {
		FEEDBACK_ERROR("Invalid paramter : new_path(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	// check symbolic link file existence
	if (stat(sym_path, &buf)) {
		FEEDBACK_ERROR("file(%s) is not presents", sym_path);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	if (unlink(sym_path) < 0) {
		FEEDBACK_LOG("unlink(%s) : %s", sym_path, strerror(errno));
	}

	if (symlink(new_path, sym_path) < 0) {
		FEEDBACK_ERROR("symlink(%s) : %s", sym_path, strerror(errno));
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

static int feedback_restore_default_file(feedback_type_e type, feedback_pattern_e pattern)
{
	const char *cur_path = NULL;
	char default_path[MAX_FILE_PATH] = {0,};
	char *temp = NULL;
	int ret = -1;

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%s)", str_type[type]);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%s)", str_pattern[pattern]);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type == FEEDBACK_TYPE_SOUND) {
		cur_path = snd_file[pattern];
	} else {
		cur_path = haptic_file[pattern];
	}

	// if there isn't cur_path, it already returns before calling this api
	if (cur_path == NULL || strlen(cur_path) == 0) {
		FEEDBACK_ERROR("Invalid parameter : cur_path(NULL)");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	temp = strcat(default_path, FEEDBACK_ORIGIN_DATA_DIR);
	strcat(temp, cur_path+strlen(FEEDBACK_DATA_DIR));
	FEEDBACK_LOG("default_path : %s", default_path);

	ret = feedback_change_symlink(cur_path, default_path);
	if (FEEDBACK_FAILED(ret)) {
		FEEDBACK_ERROR("change_symlink is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

int feedback_init(feedback_h *handle)
{
	FEEDBACK_HANDLE *phandle;
	haptic_device_h v_handle;
	xmlDocPtr v_doc;
	int ret;

	/* Sound Init */
	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) < 0) {
		FEEDBACK_ERROR("vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	if (vconf_get_bool(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, &touch_sndstatus) < 0) {
		FEEDBACK_ERROR("vconf_get_bool(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, &touch_sndstatus) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	if (vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) < 0) {
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* check vibration status */
	if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0) {
		FEEDBACK_ERROR("vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* check vib_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) < 0) {
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* check noti_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) < 0) {
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* check call status */
	if (vconf_get_int(VCONFKEY_CALL_STATE, &callstatus) < 0) {
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_CALL_STATE, &callstatus) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* xml Init */
	v_doc = xml_open(VIBRATION_XML);
	if (v_doc == NULL) {
		FEEDBACK_ERROR("xml_open(%s) fail", VIBRATION_XML);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* Vibration Init */
	ret = haptic_open(HAPTIC_DEVICE_ALL, &v_handle);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_open(HAPTIC_DEVICE_ALL, &v_handle) ==> FAIL!! : %d", ret);
		v_handle = (haptic_device_h)DEFAULT_FEEDBACK_HANDLE;
	}

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, feedback_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SOUND_STATUS, feedback_soundon_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, feedback_vibstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb, NULL);

	FEEDBACK_LOG("vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) ==> %d", sndstatus);
	FEEDBACK_LOG("vconf_get_bool(VCONFKEY_SETAPPL_....UNDS_BOOL, &touch_sndstatus) ==> %d", touch_sndstatus);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) ==> %d", soundon);
	FEEDBACK_LOG("vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> %d", vibstatus);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) ==> %d", vib_level);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) ==> %d", noti_level);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_CALL_STATUS, &callstatus) ==> %d", callstatus);

	phandle = (FEEDBACK_HANDLE *)malloc(sizeof(FEEDBACK_HANDLE));
	phandle->v_handle = v_handle;
	phandle->v_doc = v_doc;
	*handle = (feedback_h)phandle;
	FEEDBACK_LOG("handle value : %x", handle);
	return FEEDBACK_ERROR_NONE;
}

int feedback_fini(feedback_h handle)
{
	FEEDBACK_HANDLE *phandle = (FEEDBACK_HANDLE *)handle;
	int ret = -1;

	if (!handle) {
		FEEDBACK_ERROR("Invalid parameter : handle(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (phandle->v_handle != DEFAULT_FEEDBACK_HANDLE) {
		ret = haptic_close(phandle->v_handle);
		if (ret != HAPTIC_ERROR_NONE) {
			FEEDBACK_ERROR("haptic_close is failed : %d", ret);
		}
	}

	if (phandle->v_doc) {
		xml_close(phandle->v_doc);
	}

	free(phandle);

	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, feedback_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SOUND_STATUS, feedback_soundon_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, feedback_vibstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb);
	vconf_ignore_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb);

	return FEEDBACK_ERROR_NONE;
}

int feedback_play_sound(feedback_h handle, feedback_pattern_e pattern)
{
	int ret = -1;
	int retry = FEEDBACK_RETRY_CNT;
	struct stat buf;

	if (!handle) {
		FEEDBACK_ERROR("Invalid parameter : handle(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (sndstatus == 0 && !feedback_get_always_alert_case(pattern)) {
		FEEDBACK_LOG("Sound condition is OFF (sndstatus : %d)", sndstatus);
		return FEEDBACK_ERROR_NONE;
	}

	if (soundon == 1 && pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD) {
		FEEDBACK_LOG("Touch feedback sound doesn't work during playing sound");
		return FEEDBACK_ERROR_NONE;
	}

	if (touch_sndstatus == 0 && pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD) {
		FEEDBACK_LOG("Touch Sound condition is OFF and pattern is touch type (touch_sndstatus : %d, pattern : %s)", touch_sndstatus, str_pattern[pattern]);
		return FEEDBACK_ERROR_NONE;
	}

	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = feedback_get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
	}

	if (snd_file[pattern] == NULL) {
		FEEDBACK_LOG("This case(%s) does not play sound", str_pattern[pattern]);
		return FEEDBACK_ERROR_NONE;
	}

	if (stat(snd_file[pattern], &buf)) {
		FEEDBACK_ERROR("%s is not presents", snd_file[pattern]);
		ret = feedback_restore_default_file(FEEDBACK_TYPE_SOUND, pattern);
		if (FEEDBACK_FAILED(ret)) {
			FEEDBACK_ERROR("feedback_restore_default_file(%s) error", str_pattern[pattern]);
			return FEEDBACK_ERROR_OPERATION_FAILED;
		}
		FEEDBACK_LOG("%s is restored", snd_file[pattern]);
	}

	do {
		ret = mm_sound_play_keysound(snd_file[pattern], feedback_get_volume_type(pattern));
		if (ret == MM_ERROR_NONE) {
			FEEDBACK_LOG("Play success! SND filename is %s", snd_file[pattern]);
			return FEEDBACK_ERROR_NONE;
		}
		FEEDBACK_ERROR("mm_sound_play_keysound() returned error(%d)", ret);
	} while(retry--);

	return FEEDBACK_ERROR_OPERATION_FAILED;
}

int feedback_play_vibration(feedback_h handle, feedback_pattern_e pattern)
{
	FEEDBACK_HANDLE *phandle = (FEEDBACK_HANDLE *)handle;
	int ret;
	struct stat buf;
	struct xmlData *data;

	if (!handle) {
		FEEDBACK_ERROR("Invalid parameter : handle(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (handle == DEFAULT_FEEDBACK_HANDLE) {
		FEEDBACK_ERROR("haptic is not initialized");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	if (vibstatus == 0 && !feedback_get_always_alert_case(pattern))  {
		FEEDBACK_LOG("Vibration condition is OFF (vibstatus : %d)", vibstatus);
		return FEEDBACK_ERROR_NONE;
	}

	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = feedback_get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
	}

	ret = feedback_get_data(phandle->v_doc, pattern, &data);
	if (ret < 0) {
		FEEDBACK_ERROR("feedback_get_vibration_data fail");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	if (data->data == NULL) {
		FEEDBACK_LOG("This case(%s) does not play vibration", str_pattern[pattern]);
		feedback_release_data(data);
		return FEEDBACK_ERROR_NONE;
	}

	/* play haptic buffer */
	ret = haptic_vibrate_buffer_with_detail(phandle->v_handle, data->data, HAPTIC_ITERATION_ONCE,
					feedback_get_haptic_level(pattern), feedback_get_priority(pattern), NULL);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_vibrate_buffer_with_detail is failed");
		feedback_release_data(data);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	feedback_release_data(data);
	return FEEDBACK_ERROR_NONE;
}

int feedback_set_path(feedback_type_e type, feedback_pattern_e pattern, char* path)
{
	const char* cur_path = NULL;
	int ret = -1;

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%s)", str_type[type]);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%s)", str_pattern[pattern]);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (path == NULL) {
		FEEDBACK_ERROR("Invalid parameter : path(NULL)");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (access(path, F_OK) != 0) {
		FEEDBACK_ERROR("Invalid parameter : path does not exist");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type == FEEDBACK_TYPE_SOUND) {
		cur_path = snd_file[pattern];
	} else {
		cur_path = haptic_file[pattern];
	}

	if (cur_path == NULL) {
		FEEDBACK_ERROR("This pattern(%s) in this type(%s) is not supported to play", str_pattern[pattern], str_type[type]);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	ret = feedback_change_symlink(cur_path, path);
	if (FEEDBACK_FAILED(ret)) {
		FEEDBACK_ERROR("change_symlink is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

int feedback_get_path(feedback_type_e type, feedback_pattern_e pattern, char* buf, unsigned int buflen)
{
	const char* cur_path = NULL;
	int retry = FEEDBACK_RETRY_CNT;

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%s)", str_type[type]);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%s)", str_pattern[pattern]);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (buf == NULL || buflen <= 0) {
		FEEDBACK_ERROR("Invalid parameter : buf(NULL) or bufLen(%d)", buflen);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (type == FEEDBACK_TYPE_SOUND) {
		cur_path = snd_file[pattern];
	} else {
		cur_path = haptic_file[pattern];
	}

	if (cur_path == NULL) {
		FEEDBACK_ERROR("This pattern(%s) in this type(%s) is not supported to play", str_pattern[pattern], str_type[type]);
		snprintf(buf, buflen, "NULL");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	do {
		if(readlink(cur_path, buf, buflen) < 0) {
			FEEDBACK_ERROR("readlink is failed : %s", strerror(errno));
			return FEEDBACK_ERROR_OPERATION_FAILED;
		}
	} while(retry--);

	return FEEDBACK_ERROR_NONE;
}
