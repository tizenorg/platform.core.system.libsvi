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
#include "feedback-log.h"

#define FEEDBACK_RETRY_CNT       1
#define MAX_FILE_PATH          512

static int soundon = -1;
static int noti_level = -1;
static int vib_level = -1;
static int sndstatus = -1;
static int vibstatus = -1;
static int callstatus = -1;

static void __feedback_soundon_cb(keynode_t *key, void* data)
{
	soundon = vconf_keynode_get_int(key);
	FEEDBACK_LOG("[[[[[[[[[[[[[[soundon changed!! new soundon => %d", soundon);
	return;
}

static void __feedback_vib_cb(keynode_t *key, void* data)
{
	vib_level = vconf_keynode_get_int(key);
	FEEDBACK_LOG("[[[[[[[[[[[[[[vib_level changed!! new vib_level => %d", vib_level);
	return;
}

static void __feedback_noti_cb(keynode_t *key, void* data)
{
	noti_level = vconf_keynode_get_int(key);
	FEEDBACK_LOG("[[[[[[[[[[[[[[noti_level changed!! new noti_level => %d", noti_level);
	return;
}

static void __feedback_sndstatus_cb(keynode_t *key, void* data)
{
	sndstatus = vconf_keynode_get_bool(key);
	FEEDBACK_LOG("[[[[[[[[[[[[[[sndstatus changed!! new sndstatus => %d", sndstatus);
	return;
}

static void __feedback_vibstatus_cb(keynode_t *key, void* data)
{
	vibstatus = vconf_keynode_get_bool(key);
	FEEDBACK_LOG("[[[[[[[[[[[[[[vibstatus changed!! new vibstatus => %d", vibstatus);
	return;
}

static void __feedback_callstatus_cb(keynode_t *key, void* data)
{
	callstatus = vconf_keynode_get_int(key);
	FEEDBACK_LOG("[[[[[[[[[[[[[[callstatus changed!! new callstatus => %d", callstatus);
	return;
}

static feedback_pattern_e __feedback_get_alert_on_call_key(feedback_pattern_e pattern)
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

static haptic_priority_e __feedback_get_priority(feedback_pattern_e pattern)
{
	if (pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD)
		return HAPTIC_PRIORITY_MIN;

	return HAPTIC_PRIORITY_MIDDLE;
}

static volume_type_t __feedback_get_volume_type(feedback_pattern_e pattern)
{
	if (pattern == FEEDBACK_PATTERN_TAP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_TOUCH;
	else if (pattern >= FEEDBACK_PATTERN_KEY0 && pattern <= FEEDBACK_PATTERN_KEY_SHARP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_DIALER;
	else if (pattern == FEEDBACK_PATTERN_VOLUME_KEY)
		return VOLUME_TYPE_RINGTONE;

	return VOLUME_TYPE_SYSTEM;
}

static int __feedback_get_haptic_level(feedback_pattern_e pattern)
{
	int level = -1;

	if (pattern >= FEEDBACK_PATTERN_MESSAGE && pattern <= FEEDBACK_PATTERN_GENERAL_ON_CALL)
		level = noti_level;
	else
		level = vib_level;

	FEEDBACK_LOG("Call status : %d, pattern : %d, level : %d", callstatus, pattern, level);
	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = __feedback_get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %d", pattern);

		// if call status is ON, vibration magnitude is 20%
		level = (int)(level*0.2f);
		level = (level < 1) ? 1 : level;
		FEEDBACK_LOG("level changed : %d", level);
	}

	// START : Temporary code
	// Casue : Setting vconf of intensity(feedback) is between 0 and 5.
	//         the vconf will be changed but not yet.
	level = level*20;
	// END

	return level;
}

static bool __feedback_get_always_alert_case(feedback_pattern_e pattern)
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

static int __feedback_change_symlink(const char *sym_path, const char *new_path)
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

static int __feedback_restore_default_file(feedback_type_e type, feedback_pattern_e pattern)
{
	const char *cur_path = NULL;
	char default_path[MAX_FILE_PATH] = {0,};
	char *temp = NULL;
	int ret = -1;

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
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

	ret = __feedback_change_symlink(cur_path, default_path);
	if (FEEDBACK_FAILED(ret)) {
		FEEDBACK_ERROR("change_symlink is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

int _feedback_init(feedback_h *handle)
{
	haptic_device_h v_handle = NULL;
	int ret = -1;

	/* Sound Init */
	if (vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) < 0) {
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) ==> FAIL!!");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) < 0) {
		FEEDBACK_ERROR("vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) ==> FAIL!!");
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

	/* Vibration Init */
	ret = haptic_open(HAPTIC_DEVICE_ALL, &v_handle);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_open(HAPTIC_DEVICE_ALL, &v_handle) ==> FAIL!! : %d", ret);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SOUND_STATUS, __feedback_soundon_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, __feedback_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, __feedback_vibstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, __feedback_vib_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, __feedback_noti_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_CALL_STATE, __feedback_callstatus_cb, NULL);

	FEEDBACK_LOG("vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) ==> %d", soundon);
	FEEDBACK_LOG("vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) ==> %d", sndstatus);
	FEEDBACK_LOG("vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> %d", vibstatus);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) ==> %d", vib_level);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) ==> %d", noti_level);
	FEEDBACK_LOG("vconf_get_int(VCONFKEY_CALL_STATUS, &callstatus) ==> %d", callstatus);

	*handle = (feedback_h)v_handle;
	return FEEDBACK_ERROR_NONE;
}

int _feedback_fini(feedback_h handle)
{
	int ret = -1;

	if (handle <= 0) {
		FEEDBACK_ERROR("Invalid parameter : handle(%d)", handle);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	ret = haptic_close((haptic_device_h)handle);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_close is failed : %d", ret);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	vconf_ignore_key_changed(VCONFKEY_SOUND_STATUS, __feedback_soundon_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, __feedback_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, __feedback_vibstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, __feedback_vib_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, __feedback_noti_cb);
	vconf_ignore_key_changed(VCONFKEY_CALL_STATE, __feedback_callstatus_cb);

	return FEEDBACK_ERROR_NONE;
}

int _feedback_play_sound(feedback_h handle, feedback_pattern_e pattern)
{
	int ret = -1;
	int retry = FEEDBACK_RETRY_CNT;
	struct stat buf;

	if (handle <= 0) {
		FEEDBACK_ERROR("Please call _feedback_init() for sound init ");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (sndstatus == 0 && !__feedback_get_always_alert_case(pattern)) {
		FEEDBACK_LOG("Sound condition is OFF (sndstatus : %d)", sndstatus);
		return FEEDBACK_ERROR_NONE;
	}

	if (soundon == 1 && pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD) {
		FEEDBACK_LOG("Touch feedback sound doesn't work during playing sound");
		return FEEDBACK_ERROR_NONE;
	}

	if (pattern == FEEDBACK_PATTERN_NONE) {
		FEEDBACK_LOG("call _feedback_play_sound passing FEEDBACK_PATTERN_NONE");
		return FEEDBACK_ERROR_NONE;
	}

	if (snd_file[pattern] == NULL) {
		FEEDBACK_LOG("This case(%d) does not play sound", pattern);
		return FEEDBACK_ERROR_NONE;
	}

	if (stat(snd_file[pattern], &buf)) {
		FEEDBACK_ERROR("%s is not presents", snd_file[pattern]);
		ret = __feedback_restore_default_file(FEEDBACK_TYPE_SOUND, pattern);
		if (FEEDBACK_FAILED(ret)) {
			FEEDBACK_ERROR("__feedback_restore_default_file(%d/%d) error", FEEDBACK_TYPE_SOUND, pattern);
			return FEEDBACK_ERROR_OPERATION_FAILED;
		}
		FEEDBACK_LOG("%s is restored", snd_file[pattern]);
	}

	FEEDBACK_LOG("Call status : %d, pattern : %d", callstatus, pattern);
	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = __feedback_get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %d", pattern);
	}

	do {
		ret = mm_sound_play_keysound(snd_file[pattern], __feedback_get_volume_type(pattern));
		if (ret == MM_ERROR_NONE) {
			FEEDBACK_LOG("Play success! SND filename is %s", snd_file[pattern]);
			return FEEDBACK_ERROR_NONE;
		}
		FEEDBACK_ERROR("mm_sound_play_keysound() returned error(%d)", ret);
	} while(retry--);

	return FEEDBACK_ERROR_OPERATION_FAILED;
}

int _feedback_play_vibration(feedback_h handle, feedback_pattern_e pattern)
{
	int ret = -1;
	struct stat buf;

	if (handle <= 0) {
		FEEDBACK_ERROR("Please call _feedback_init() for sound init ");
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (vibstatus == 0 && !__feedback_get_always_alert_case(pattern))  {
		FEEDBACK_LOG("Vibration condition is OFF (vibstatus : %d)", vibstatus);
		return FEEDBACK_ERROR_NONE;
	}

	if (pattern == FEEDBACK_PATTERN_NONE) {
		FEEDBACK_LOG("call _feedback_play_vibration passing FEEDBACK_PATTERN_NONE");
		return FEEDBACK_ERROR_NONE;
	}

	if (haptic_file[pattern] == NULL) {
		FEEDBACK_LOG("This case(%d) does not play vibration", pattern);
		return FEEDBACK_ERROR_NONE;
	}

	if (stat(haptic_file[pattern], &buf)) {
		FEEDBACK_ERROR("%s is not presents", haptic_file[pattern]);
		ret = __feedback_restore_default_file(FEEDBACK_TYPE_VIBRATION, pattern);
		if (FEEDBACK_FAILED(ret)) {
			FEEDBACK_ERROR("__feedback_restore_default_file(%d/%d) error", FEEDBACK_TYPE_VIBRATION, pattern);
			return FEEDBACK_ERROR_OPERATION_FAILED;
		}
		FEEDBACK_LOG("%s is restored", haptic_file[pattern]);
	}

	ret = haptic_vibrate_file_with_detail((haptic_device_h)handle, haptic_file[pattern], HAPTIC_ITERATION_ONCE, __feedback_get_haptic_level(pattern), __feedback_get_priority(pattern), NULL);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_vibrate_file_with_detail(%s) is failed", haptic_file[pattern]);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

int _feedback_set_path(feedback_type_e type, feedback_pattern_e pattern, char* path)
{
	const char* cur_path = NULL;
	int ret = -1;

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
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
		FEEDBACK_ERROR("This pattern(%d) in this type(%d) is not supported to play", pattern, type);
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	ret = __feedback_change_symlink(cur_path, path);
	if (FEEDBACK_FAILED(ret)) {
		FEEDBACK_ERROR("change_symlink is failed");
		return FEEDBACK_ERROR_OPERATION_FAILED;
	}

	return FEEDBACK_ERROR_NONE;
}

int _feedback_get_path(feedback_type_e type, feedback_pattern_e pattern, char* buf, unsigned int buflen)
{
	const char* cur_path = NULL;
	int retry = FEEDBACK_RETRY_CNT;

	if (type <= FEEDBACK_TYPE_NONE || type >= FEEDBACK_TYPE_END) {
		FEEDBACK_ERROR("Invalid parameter : type(%d)", type);
		return FEEDBACK_ERROR_INVALID_PARAMETER;
	}

	if (pattern <= FEEDBACK_PATTERN_NONE || pattern >= FEEDBACK_PATTERN_END) {
		FEEDBACK_ERROR("Invalid parameter : pattern(%d)", pattern);
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
		FEEDBACK_ERROR("This pattern(%d) in this type(%d) is not supported to play", pattern, type);
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
