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

#define FEEDBACK_SOUND_DIR			FEEDBACK_DATA_DIR"/sound"
#define FEEDBACK_SOUND_TOUCH_DIR	FEEDBACK_SOUND_DIR"/touch"
#define FEEDBACK_SOUND_OPER_DIR		FEEDBACK_SOUND_DIR"/operation"

static const char* sound_file[] = {
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : GENERAL */
	FEEDBACK_SOUND_TOUCH_DIR"/touch.wav",
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : TEXT_NUMERIC_INPUT */
	FEEDBACK_SOUND_TOUCH_DIR"/sip.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/sip_backspace.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/sip.wav",
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : DAILER */
	FEEDBACK_SOUND_TOUCH_DIR"/key0.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key1.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key2.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key3.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key4.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key5.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key6.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key7.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key8.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/key9.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/keyasterisk.wav",
	FEEDBACK_SOUND_TOUCH_DIR"/keysharp.wav",
	/* TOUCH : H/W OR SOFT TOUCH : HOLD(TAP & HOLD) */
	NULL,
	/* TOUCH : H/W OR SOFT TOUCH : MULTI TAP */
	NULL,
	/* TOUCH : H/W OR SOFT TOUCH : TAP */
	NULL,
	/* TOUCH : H/W OR SOFT TOUCH : TAP & HOLD */
	NULL,

	/* NOTIFICATION : INCOMING : MESSAGE */
	NULL,
	/* NOTIFICATION : INCOMING : MESSAGE ALERT ON CALL */
	NULL,
	/* NOTIFICATION : INCOMING : EMAIL */
	NULL,
	/* NOTIFICATION : INCOMING : EMAIL ALERT ON CALL */
	NULL,
	/* NOTIFICATION : ALARM : WAKEUP */
	NULL,
	/* NOTIFICATION : ALARM : WAKEUP ALERT ON CALL */
	NULL,
	/* NOTIFICATION : ALARM : SCHEDULE */
	NULL,
	/* NOTIFICATION : ALARM : SCHEDULE ALERT ON CALL */
	NULL,
	/* NOTIFICATION : ALARM : TIMER */
	NULL,
	/* NOTIFICATION : ALARM : TIMER ALERT ON CALL */
	NULL,
	/* NOTIFICATION : GENERAL(TICKER/IM/SMS ETC) */
	FEEDBACK_SOUND_OPER_DIR"/call_connect.wav",
	/* NOTIFICATION : GENERAL(TICKER/IM/SMS ETC) ALERT ON CALL */
	FEEDBACK_SOUND_OPER_DIR"/call_connect.wav",

	/* OPERATION : POWER ON/OFF */
	FEEDBACK_SOUND_OPER_DIR"/power_on.wav",
	NULL,
	/* OPERATION : CHARGECONN */
	FEEDBACK_SOUND_OPER_DIR"/charger_connection.wav",
	/* OPERATION : CHARGECONN ALERT ON CALL */
	FEEDBACK_SOUND_OPER_DIR"/charger_connection.wav",
	/* OPERATION : FULLCHAREGED */
	FEEDBACK_SOUND_OPER_DIR"/fully_charged.wav",
	/* OPERATION : FULLCHAREGED ALERT ON CALL */
	FEEDBACK_SOUND_OPER_DIR"/fully_charged.wav",
	/* OPERATION : LOW BATTERY */
	FEEDBACK_SOUND_OPER_DIR"/low_battery.wav",
	/* OPERATION : LOW BATTERY ALERT ON CALL */
	FEEDBACK_SOUND_OPER_DIR"/low_battery.wav",
	/* OPERATION : LOCK/UNLOCK */
	FEEDBACK_SOUND_OPER_DIR"/lock.wav",
	FEEDBACK_SOUND_OPER_DIR"/unlock.wav",
	/* OPERATION : CALL CONNECT/ DISCONNECT */
	FEEDBACK_SOUND_OPER_DIR"/call_connect.wav",
	FEEDBACK_SOUND_OPER_DIR"/call_disconnect.wav",
	/* OPERATION : MINUTE MINDER */
	FEEDBACK_SOUND_OPER_DIR"/minute_minder.wav",
	/* OPERATION : VIBRATION */
	NULL,
	/* OPERATION : CAMERA SHUTTER / SCREEN CAPTURE */
	FEEDBACK_SOUND_OPER_DIR"/shutter.wav",
	/* OPERATION : LIST RE-ORDER */
	FEEDBACK_SOUND_OPER_DIR"/list_reorder.wav",
	/* OPERATION : LIST SLIDER */
	FEEDBACK_SOUND_OPER_DIR"/slider_sweep.wav",
	/* OPERATION : VOLUME KEY */
	FEEDBACK_SOUND_OPER_DIR"/volume_control.wav",
};

static int sndstatus;
static int touch_sndstatus;
static int soundon;

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

static volume_type_t get_volume_type(feedback_pattern_e pattern)
{
	if (pattern == FEEDBACK_PATTERN_TAP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_TOUCH;
	else if (pattern >= FEEDBACK_PATTERN_KEY0 && pattern <= FEEDBACK_PATTERN_KEY_SHARP)
		return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_DIALER;
	else if (pattern == FEEDBACK_PATTERN_VOLUME_KEY)
		return VOLUME_TYPE_RINGTONE;

	return VOLUME_TYPE_SYSTEM;
}

static bool get_always_alert_case(feedback_pattern_e pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_WAKEUP_ON_CALL:
		return true;
	default:
		break;
	}
	return false;
}

static int change_symlink(const char *sym_path, const char *new_path)
{
	struct stat buf;

	assert(sym_path != NULL && strlen(sym_path));
	assert(new_path != NULL && strlen(new_path));

	/* check symbolic link file existence */
	if (stat(sym_path, &buf)) {
		_E("file(%s) is not presents", sym_path);
		return -EPERM;
	}

	if (unlink(sym_path) < 0)
		_D("unlink(%s) : %s", sym_path, strerror(errno));

	if (symlink(new_path, sym_path) < 0) {
		_E("symlink(%s) : %s", sym_path, strerror(errno));
		return -EPERM;
	}

	return 0;
}

static int restore_default_file(feedback_pattern_e pattern)
{
	char default_path[PATH_MAX] = {0,};
	const char *cur_path;
	char *temp;
	int ret;

	cur_path = sound_file[pattern];
	/* if there isn't cur_path, it already returns before calling this api */
	if (cur_path == NULL || strlen(cur_path) == 0) {
		_E("Invalid parameter : cur_path(NULL)");
		return -EPERM;
	}

	temp = strcat(default_path, FEEDBACK_ORIGIN_DATA_DIR);
	strcat(temp, cur_path+strlen(FEEDBACK_DATA_DIR));
	_D("default_path : %s", default_path);

	ret = change_symlink(cur_path, default_path);
	if (ret < 0) {
		_E("change_symlink is failed");
		return -EPERM;
	}

	return 0;
}

static void sound_init(void)
{
	/* Sound Init */
	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_SOUND_STATUS_BOOL ==> FAIL!!");

	if (vconf_get_bool(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, &touch_sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL ==> FAIL!!");

	if (vconf_get_int(VCONFKEY_SOUND_STATUS, &soundon) < 0)
		_W("VCONFKEY_SOUND_STATUS ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, feedback_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SOUND_STATUS, feedback_soundon_cb, NULL);
}

static void sound_exit(void)
{
	/* remove watch */
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, feedback_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SOUND_STATUS, feedback_soundon_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_SOUNDS_BOOL, feedback_touch_sndstatus_cb);
}

static int sound_play(feedback_pattern_e pattern)
{
	struct stat buf;
	int retry = FEEDBACK_RETRY_CNT, ret;

	if (sndstatus == 0 && !get_always_alert_case(pattern)) {
		_D("Sound condition is OFF (sndstatus : %d)", sndstatus);
		return 0;
	}

	if (soundon == 1 && pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD) {
		_D("Touch feedback sound doesn't work during playing sound");
		return 0;
	}

	if (touch_sndstatus == 0 && pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD) {
		_D("Touch Sound condition is OFF and pattern is touch type (touch_sndstatus : %d, pattern : %s)", touch_sndstatus, str_pattern[pattern]);
		return 0;
	}

	if (sound_file[pattern] == NULL) {
		_D("This case(%s) does not play sound", str_pattern[pattern]);
		return 0;
	}

	if (stat(sound_file[pattern], &buf)) {
		_E("%s is not presents", sound_file[pattern]);
		ret = restore_default_file(pattern);
		if (ret < 0) {
			_E("restore_default_file(%s) error", str_pattern[pattern]);
			return -EPERM;
		}
		_D("%s is restored", sound_file[pattern]);
	}

	do {
		ret = mm_sound_play_keysound(sound_file[pattern], get_volume_type(pattern));
		if (ret == MM_ERROR_NONE) {
			_D("Play success! SND filename is %s", sound_file[pattern]);
			return 0;
		}
		_E("mm_sound_play_keysound() returned error(%d)", ret);
	} while(retry--);

	return -EPERM;
}

static int sound_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	const char *cur_path;
	int retry = FEEDBACK_RETRY_CNT;

	assert(buf != NULL && buflen > 0);

	cur_path = sound_file[pattern];
	if (cur_path == NULL) {
		_E("This pattern(%s) in sound type is not supported to play", str_pattern[pattern]);
		snprintf(buf, buflen, "NULL");
		return -ENOENT;
	}

	do {
		if(readlink(cur_path, buf, buflen) < 0) {
			_E("readlink is failed : %s", strerror(errno));
			return -EPERM;
		}
	} while(retry--);

	return 0;
}

static int sound_set_path(feedback_pattern_e pattern, char *path)
{
	const char *cur_path;
	int ret;

	assert(path != NULL);

	if (access(path, F_OK) != 0) {
		_E("Invalid parameter : path does not exist");
		return -ENOENT;
	}

	cur_path = sound_file[pattern];
	if (cur_path == NULL) {
		_E("This pattern(%s) in sound type is not supported to play", str_pattern[pattern]);
		return -ENOENT;
	}

	ret = change_symlink(cur_path, path);
	if (ret < 0) {
		_E("change_symlink is failed");
		return -EPERM;
	}

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
