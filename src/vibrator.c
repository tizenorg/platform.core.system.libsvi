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
#include <haptic.h>

#include "feedback.h"
#include "feedback-internal.h"
#include "feedback-str.h"
#include "feedback-log.h"
#include "xmlparser.h"
#include "devices.h"

#define FEEDBACK_HAPTIC_DIR			FEEDBACK_DATA_DIR"/haptic"
#define FEEDBACK_HAPTIC_TOUCH_DIR	FEEDBACK_HAPTIC_DIR"/touch"
#define FEEDBACK_HAPTIC_OPER_DIR	FEEDBACK_HAPTIC_DIR"/operation"
#define FEEDBACK_HAPTIC_NOTI_DIR	FEEDBACK_HAPTIC_DIR"/notification"
#define FEEDBACK_HAPTIC_DEFAULT_DIR FEEDBACK_HAPTIC_DIR"/default"

static const char* haptic_file[] = {
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : GENERAL */
	NULL,
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : TEXT_NUMERIC_INPUT */
	NULL,
	NULL,
	NULL,
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : DAILER */
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	/* TOUCH : H/W OR SOFT TOUCH : HOLD(TAP & HOLD) */
	FEEDBACK_HAPTIC_TOUCH_DIR"/touch.tht",
	/* TOUCH : H/W OR SOFT TOUCH : MULTI TAP */
	FEEDBACK_HAPTIC_TOUCH_DIR"/touch.tht",
	/* TOUCH : H/W OR SOFT TOUCH : TAP */
	NULL,
	/* TOUCH : H/W OR SOFT TOUCH : TAP & HOLD */
	NULL,

	/* NOTIFICATION : INCOMING : MESSAGE */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : INCOMING : MESSAGE ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : INCOMING : EMAIL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : INCOMING : EMAIL ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : ALARM : WAKEUP */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : ALARM : WAKEUP ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : ALARM : SCHEDULE */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : ALARM : SCHEDULE ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : ALARM : TIMER */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : ALARM : TIMER ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : GENERAL(TICKER/IM/SMS ETC) */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* NOTIFICATION : GENERAL(TICKER/IM/SMS ETC) ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",

	/* OPERATION : POWER ON/OFF */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : CHARGECONN */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : CHARGECONN ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : FULLCHAREGED */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : FULLCHAREGED ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : LOW BATTERY */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : LOW BATTERY ALERT ON CALL */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : LOCK/UNLOCK */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : CALL CONNECT/ DISCONNECT */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : MINUTE MINDER */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : VIBRATION */
	FEEDBACK_HAPTIC_DEFAULT_DIR"/Basic_call.tht",
	/* OPERATION : CAMERA SHUTTER / SCREEN CAPTURE */
	NULL,
	/* OPERATION : LIST RE-ORDER */
	NULL,
	/* OPERATION : LIST SLIDER */
	NULL,
	/* OPERATION : VOLUME KEY */
	NULL,
};

#define VIBRATION_XML				"/usr/share/feedback/vibration.xml"

static int vibstatus;
static int vib_level;
static int noti_level;

static haptic_device_h v_handle;
static xmlDocPtr v_doc;

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

static haptic_priority_e get_priority(feedback_pattern_e pattern)
{
	if (pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD)
		return HAPTIC_PRIORITY_MIN;

	return HAPTIC_PRIORITY_MIDDLE;
}

static int get_haptic_level(feedback_pattern_e pattern)
{
	int level;

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

static bool get_always_alert_case(feedback_pattern_e pattern)
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

static int get_xml_data(xmlDocPtr doc, feedback_pattern_e pattern, struct xmlData **data)
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

static void release_xml_data(struct xmlData *data)
{
	if (data == NULL)
		return;

	xml_free(data);
}

static int change_symlink(const char *sym_path, const char *new_path)
{
	struct stat buf;

	assert(sym_path != NULL && strlen(sym_path));
	assert(new_path != NULL && strlen(new_path));

	/* check symbolic link file existence */
	if (stat(sym_path, &buf)) {
		FEEDBACK_ERROR("file(%s) is not presents", sym_path);
		return -EPERM;
	}

	if (unlink(sym_path) < 0)
		FEEDBACK_LOG("unlink(%s) : %s", sym_path, strerror(errno));

	if (symlink(new_path, sym_path) < 0) {
		FEEDBACK_ERROR("symlink(%s) : %s", sym_path, strerror(errno));
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

	cur_path = haptic_file[pattern];
	// if there isn't cur_path, it already returns before calling this api
	if (cur_path == NULL || strlen(cur_path) == 0) {
		FEEDBACK_ERROR("Invalid parameter : cur_path(NULL)");
		return -EPERM;
	}

	temp = strcat(default_path, FEEDBACK_ORIGIN_DATA_DIR);
	strcat(temp, cur_path+strlen(FEEDBACK_DATA_DIR));
	FEEDBACK_LOG("default_path : %s", default_path);

	ret = change_symlink(cur_path, default_path);
	if (ret < 0) {
		FEEDBACK_ERROR("change_symlink is failed");
		return -EPERM;
	}

	return 0;
}

static void vibrator_init(void)
{
	int ret;

	/* xml Init */
	v_doc = xml_open(VIBRATION_XML);
	if (v_doc == NULL) {
		FEEDBACK_ERROR("xml_open(%s) fail", VIBRATION_XML);
		return;
	}

	/* Vibration Init */
	ret = haptic_open(HAPTIC_DEVICE_ALL, &v_handle);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_open(HAPTIC_DEVICE_ALL, &v_handle) ==> FAIL!! : %d", ret);
		xml_close(v_doc);
		v_doc = NULL;
		return;
	}

	/* check vibration status */
	if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0)
		FEEDBACK_ERROR("vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> FAIL!!");

	/* check vib_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) < 0)
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) ==> FAIL!!");

	/* check noti_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) < 0)
		FEEDBACK_ERROR("vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, feedback_vibstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb, NULL);
}

static void vibrator_exit(void)
{
	int ret;

	/* remove watch */
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, feedback_vibstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb);

	if (v_handle) {
		ret = haptic_close(v_handle);
		if (ret != HAPTIC_ERROR_NONE)
			FEEDBACK_ERROR("haptic_close is failed : %d", ret);
		v_handle = NULL;
	}

	if (v_doc) {
		xml_close(v_doc);
		v_doc = NULL;
	}
}

static int vibrator_play(feedback_pattern_e pattern)
{
	int ret;
	struct xmlData *data;

	if (!v_handle || !v_doc) {
		FEEDBACK_ERROR("Not initialize");
		return -EPERM;
	}

	if (vibstatus == 0 && !get_always_alert_case(pattern))  {
		FEEDBACK_LOG("Vibration condition is OFF (vibstatus : %d)", vibstatus);
		return 0;
	}

	if (callstatus != VCONFKEY_CALL_OFF) {
		pattern = feedback_get_alert_on_call_key(pattern);
		FEEDBACK_LOG("Call status is connected or connecting. pattern changed : %s", str_pattern[pattern]);
	}

	ret = get_xml_data(v_doc, pattern, &data);
	if (ret < 0) {
		FEEDBACK_ERROR("feedback_get_vibration_data fail");
		return -EPERM;
	}

	if (data->data == NULL) {
		FEEDBACK_LOG("This case(%s) does not play vibration", str_pattern[pattern]);
		release_xml_data(data);
		return 0;
	}

	/* play haptic buffer */
	ret = haptic_vibrate_buffer_with_detail(v_handle, data->data, HAPTIC_ITERATION_ONCE,
					get_haptic_level(pattern), get_priority(pattern), NULL);
	if (ret != HAPTIC_ERROR_NONE) {
		FEEDBACK_ERROR("haptic_vibrate_buffer_with_detail is failed");
		release_xml_data(data);
		return -EPERM;
	}

	release_xml_data(data);
	return 0;
}

static int vibrator_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	const char *cur_path;
	int retry = FEEDBACK_RETRY_CNT;

	assert(buf != NULL && buflen > 0);

	cur_path = haptic_file[pattern];
	if (cur_path == NULL) {
		FEEDBACK_ERROR("This pattern(%s) in vibrator type is not supported to play", str_pattern[pattern]);
		snprintf(buf, buflen, "NULL");
		return -ENOENT;
	}

	do {
		if(readlink(cur_path, buf, buflen) < 0) {
			FEEDBACK_ERROR("readlink is failed : %s", strerror(errno));
			return -EPERM;
		}
	} while(retry--);

	return 0;
}

static int vibrator_set_path(feedback_pattern_e pattern, char *path)
{
	const char *cur_path;
	int ret;

	assert(path != NULL);

	if (access(path, F_OK) != 0) {
		FEEDBACK_ERROR("Invalid parameter : path does not exist");
		return -ENOENT;
	}

	cur_path = haptic_file[pattern];
	if (cur_path == NULL) {
		FEEDBACK_ERROR("This pattern(%s) in vibrator type is not supported to play", str_pattern[pattern]);
		return -ENOENT;
	}

	ret = change_symlink(cur_path, path);
	if (ret < 0) {
		FEEDBACK_ERROR("change_symlink is failed");
		return -EPERM;
	}

	return 0;
}

static const struct device_ops vibrator_device_ops = {
	.type = FEEDBACK_TYPE_VIBRATION,
	.init = vibrator_init,
	.exit = vibrator_exit,
	.play = vibrator_play,
	.get_path = vibrator_get_path,
	.set_path = vibrator_set_path,
};

DEVICE_OPS_REGISTER(&vibrator_device_ops);