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
#include <vconf.h>

#include "feedback.h"
#include "feedback-ids-mobile.h"
#include "profiles.h"
#include "log.h"

/* Temporary keys */
#ifndef VCONFKEY_SETAPPL_VIBRATE_WHEN_NOTIFICATION_BOOL
#define VCONFKEY_SETAPPL_VIBRATE_WHEN_NOTIFICATION_BOOL "db/setting/sound/vibrate_when_notification"
#endif
#ifndef VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL
#define VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL "db/setting/sound/haptic_feedback_on"
#endif


static const char *mobile_str_type[] =
{
	"FEEDBACK_TYPE_NONE",
	"FEEDBACK_TYPE_SOUND",
	"FEEDBACK_TYPE_VIBRATION",
	"FEEDBACK_TYPE_MOBILE_LED",
};

static const char *mobile_str_pattern[] =
{
	"FEEDBACK_PATTERN_TAP",
	"FEEDBACK_PATTERN_SIP",
	"FEEDBACK_PATTERN_KEY0",
	"FEEDBACK_PATTERN_KEY1",
	"FEEDBACK_PATTERN_KEY2",
	"FEEDBACK_PATTERN_KEY3",
	"FEEDBACK_PATTERN_KEY4",
	"FEEDBACK_PATTERN_KEY5",
	"FEEDBACK_PATTERN_KEY6",
	"FEEDBACK_PATTERN_KEY7",
	"FEEDBACK_PATTERN_KEY8",
	"FEEDBACK_PATTERN_KEY9",
	"FEEDBACK_PATTERN_KEY_STAR",
	"FEEDBACK_PATTERN_KEY_SHARP",
	"FEEDBACK_PATTERN_KEY_BACK",
	"FEEDBACK_PATTERN_HOLD",
	"FEEDBACK_PATTERN_HW_TAP",
	"FEEDBACK_PATTERN_HW_HOLD",

	"FEEDBACK_PATTERN_MESSAGE",
	"FEEDBACK_PATTERN_EMAIL",
	"FEEDBACK_PATTERN_WAKEUP",
	"FEEDBACK_PATTERN_SCHEDULE",
	"FEEDBACK_PATTERN_TIMER",
	"FEEDBACK_PATTERN_GENERAL",

	"FEEDBACK_PATTERN_POWERON",
	"FEEDBACK_PATTERN_POWEROFF",
	"FEEDBACK_PATTERN_CHARGERCONN",
	"FEEDBACK_PATTERN_CHARGING_ERROR",
	"FEEDBACK_PATTERN_FULLCHARGED",
	"FEEDBACK_PATTERN_LOWBATT",
	"FEEDBACK_PATTERN_LOCK",
	"FEEDBACK_PATTERN_UNLOCK",
	"FEEDBACK_PATTERN_VIBRATION_ON",
	"FEEDBACK_PATTERN_SILENT_OFF",
	"FEEDBACK_PATTERN_BT_CONNECTED",
	"FEEDBACK_PATTERN_BT_DISCONNECTED",
	"FEEDBACK_PATTERN_LIST_REORDER",
	"FEEDBACK_PATTERN_LIST_SLIDER",
	"FEEDBACK_PATTERN_VOLUME_KEY",

	"FEEDBACK_PATTERN_MOBILE_SIP_BACKSPACE",
	"FEEDBACK_PATTERN_MOBILE_SIP_FUNCTION",
	"FEEDBACK_PATTERN_MOBILE_SIP_FJKEY",
	"FEEDBACK_PATTERN_MOBILE_MAX_CHARACTER",
	"FEEDBACK_PATTERN_MOBILE_MULTI_TAP",

	"FEEDBACK_PATTERN_MOBILE_MESSAGE_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_EMAIL_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_WAKEUP_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_SCHEDULE_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_TIMER_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_GENERAL_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_SMART_ALERT",

	"FEEDBACK_PATTERN_MOBILE_CHARGERCONN_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_CHARGING_ERROR_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_FULLCHARGED_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_LOWBATT_ON_CALL",
	"FEEDBACK_PATTERN_MOBILE_LOCK_SWIPE",
	"FEEDBACK_PATTERN_MOBILE_UNLOCK_SWIPE",
	"FEEDBACK_PATTERN_MOBILE_GEOMETRIC_LOCK",
	"FEEDBACK_PATTERN_MOBILE_CALLCONNECT",
	"FEEDBACK_PATTERN_MOBILE_DISCALLCONNECT",
	"FEEDBACK_PATTERN_MOBILE_OUTGOING_CALL",
	"FEEDBACK_PATTERN_MOBILE_MINUTEMINDER",
	"FEEDBACK_PATTERN_MOBILE_BT_PAIRING",
	"FEEDBACK_PATTERN_MOBILE_BT_WAITING",
	"FEEDBACK_PATTERN_MOBILE_SCREEN_CAPTURE",
	"FEEDBACK_PATTERN_MOBILE_MMS",
	"FEEDBACK_PATTERN_MOBILE_HOURLY_ALERT",

	"FEEDBACK_PATTERN_MOBILE_SAFETY_ALERT",
	"FEEDBACK_PATTERN_MOBILE_ACCIDENT_DETECT",
	"FEEDBACK_PATTERN_MOBILE_SEND_SOS_MESSAGE",
	"FEEDBACK_PATTERN_MOBILE_END_SOS_MESSAGE",
	"FEEDBACK_PATTERN_MOBILE_EMERGENCY_BUZZER",
	"FEEDBACK_PATTERN_MOBILE_SAFETY_LOW_POWER",
	"FEEDBACK_PATTERN_MOBILE_CMAS",
};

static int callstatus;
static int lock_sndstatus;
static int camerastatus;
static int shutter_sndstatus;
static int noti_vibstatus;
static int vib_level;
static int noti_level;
static int feedbackstatus;

static void feedback_callstatus_cb(keynode_t *key, void* data)
{
	callstatus = vconf_keynode_get_int(key);
}

static void feedback_lock_sndstatus_cb(keynode_t *key, void* data)
{
	lock_sndstatus = vconf_keynode_get_bool(key);
}

static void feedback_camerastatus_cb(keynode_t *key, void* data)
{
	camerastatus = vconf_keynode_get_int(key);
}

static void feedback_noti_vibstatus_cb(keynode_t *key, void* data)
{
	noti_vibstatus = vconf_keynode_get_bool(key);
}

static void feedback_vib_cb(keynode_t *key, void* data)
{
	vib_level = vconf_keynode_get_int(key);
}

static void feedback_noti_cb(keynode_t *key, void* data)
{
	noti_level = vconf_keynode_get_int(key);
}

static void feedback_feedbackstatus_cb(keynode_t *key, void* data)
{
	feedbackstatus = vconf_keynode_get_bool(key);
}

static void mobile_init(void)
{
	/* check call status */
	if (vconf_get_int(VCONFKEY_CALL_STATE, &callstatus) < 0)
		_W("VCONFKEY_CALL_STATE ==> FAIL!!");

	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_LOCK_BOOL, &lock_sndstatus) < 0)
		_W("VCONFKEY_SETAPPL_SOUND_LOCK_BOOL ==> FAIL!!");

	/* check camera status */
	if (vconf_get_int(VCONFKEY_CAMERA_STATE, &camerastatus) < 0)
		_W("VCONFKEY_CAMERA_STATE ==> FAIL!!");

	/* shutter sound policy */
	// This vconf is read just once, because this value is not changed in running time.
	if (vconf_get_int(VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY, &shutter_sndstatus) < 0)
		_W("VCONFKEY_CAMERA_SHUTTER_SOUND_POLICY ==> FAIL!!");

	/* check vibration status */
	if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATE_WHEN_NOTIFICATION_BOOL, &noti_vibstatus) < 0)
		_W("VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL ==> FAIL!!");

	/* check vib_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) < 0)
		_W("VCONFKEY_FEEDBACK_VIBRATION_LEVEL_INT ==> FAIL!!");

	/* check noti_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) < 0)
		_W("VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT ==> FAIL!!");

	/* feedback Init */
	if(vconf_get_bool(VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL, &feedbackstatus) < 0)
		_W("VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_LOCK_BOOL, feedback_lock_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_CAMERA_STATE, feedback_camerastatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATE_WHEN_NOTIFICATION_BOOL, feedback_noti_vibstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL, feedback_feedbackstatus_cb, NULL);
}

static void mobile_exit(void)
{
	/* remove watch */
	vconf_ignore_key_changed(VCONFKEY_CALL_STATE, feedback_callstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_LOCK_BOOL, feedback_lock_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_CAMERA_STATE, feedback_camerastatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATE_WHEN_NOTIFICATION_BOOL, feedback_noti_vibstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_HAPTIC_FEEDBACK_STATUS_BOOL, feedback_feedbackstatus_cb);
}

static bool mobile_get_switched_pattern(int pattern, int *switched)
{
	if (!switched)
		return false;

	if (callstatus == VCONFKEY_CALL_OFF)
		return false;

	/* in case of call connected or connecting */
	_D("Call status is connected or connecting.");
	if (pattern == FEEDBACK_PATTERN_MESSAGE)
		*switched = FEEDBACK_PATTERN_MOBILE_MESSAGE_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_EMAIL)
		*switched = FEEDBACK_PATTERN_MOBILE_EMAIL_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_WAKEUP)
		*switched = FEEDBACK_PATTERN_MOBILE_WAKEUP_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_SCHEDULE)
		*switched = FEEDBACK_PATTERN_MOBILE_SCHEDULE_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_TIMER)
		*switched = FEEDBACK_PATTERN_MOBILE_TIMER_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_GENERAL)
		*switched = FEEDBACK_PATTERN_MOBILE_GENERAL_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_CHARGERCONN)
		*switched = FEEDBACK_PATTERN_MOBILE_CHARGERCONN_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_CHARGING_ERROR)
		*switched = FEEDBACK_PATTERN_MOBILE_CHARGING_ERROR_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_FULLCHARGED)
		*switched = FEEDBACK_PATTERN_MOBILE_FULLCHARGED_ON_CALL;
	else if (pattern == FEEDBACK_PATTERN_LOWBATT)
		*switched = FEEDBACK_PATTERN_MOBILE_LOWBATT_ON_CALL;

	return true;
}

static bool mobile_get_always_alert_case(int type, int pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_KEY0 ... FEEDBACK_PATTERN_KEY_BACK:
	case FEEDBACK_PATTERN_HOLD:
		if (CHECK_VIBRATION(type) && feedbackstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_SIP:
	case FEEDBACK_PATTERN_MOBILE_SIP_BACKSPACE:
	case FEEDBACK_PATTERN_MOBILE_SIP_FUNCTION:
	case FEEDBACK_PATTERN_MOBILE_SIP_FJKEY:
		if (CHECK_VIBRATION(type))
			return true;
		break;
	case FEEDBACK_PATTERN_MESSAGE:
	case FEEDBACK_PATTERN_EMAIL:
		if (CHECK_VIBRATION(type) && noti_vibstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_TIMER:
	case FEEDBACK_PATTERN_MOBILE_TIMER_ON_CALL:
		if (CHECK_VIBRATION(type))
			return true;
		break;
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_MOBILE_WAKEUP_ON_CALL:
		return true;
	case FEEDBACK_PATTERN_MOBILE_MESSAGE_ON_CALL:
	case FEEDBACK_PATTERN_MOBILE_EMAIL_ON_CALL:
	case FEEDBACK_PATTERN_MOBILE_GENERAL_ON_CALL:
		break;
	case FEEDBACK_PATTERN_MOBILE_CHARGERCONN_ON_CALL:
	case FEEDBACK_PATTERN_MOBILE_CHARGING_ERROR_ON_CALL:
	case FEEDBACK_PATTERN_MOBILE_LOWBATT_ON_CALL:
	case FEEDBACK_PATTERN_MOBILE_MMS:
	case FEEDBACK_PATTERN_MOBILE_HOURLY_ALERT:
		/* no matter sound profile */
		if (CHECK_VIBRATION(type))
			return true;
		break;
	case FEEDBACK_PATTERN_MOBILE_SMART_ALERT:
	case FEEDBACK_PATTERN_MOBILE_SEND_SOS_MESSAGE:
	case FEEDBACK_PATTERN_MOBILE_END_SOS_MESSAGE:
	case FEEDBACK_PATTERN_MOBILE_CMAS:
	case FEEDBACK_PATTERN_MOBILE_OUTGOING_CALL:
		return true;
	case FEEDBACK_PATTERN_MOBILE_SCREEN_CAPTURE:
		if (CHECK_SOUND(type) && camerastatus && shutter_sndstatus)
			return true;
		break;
	default:
		break;
	}
	return false;
}

static bool mobile_get_always_off_case(int type, int pattern)
{
	int ret;

	/* check if the state of voice recorder is recording */
	if (vconf_get_int(VCONFKEY_SOUND_STATUS, &ret) < 0) {
		_D("fail to get media sound status, status will be zero");
		ret = 0;
	}

	if (CHECK_SOUND(type) && ret & VCONFKEY_SOUND_STATUS_AVRECORDING) {
		_D("voice recording status is RECORDING");
		return true;
	}

	switch (pattern) {
	case FEEDBACK_PATTERN_TAP ... FEEDBACK_PATTERN_SIP:
	case FEEDBACK_PATTERN_HOLD ... FEEDBACK_PATTERN_HW_HOLD:
	case FEEDBACK_PATTERN_MOBILE_SIP_BACKSPACE ... FEEDBACK_PATTERN_MOBILE_MAX_CHARACTER:
		if (CHECK_SOUND(type) && !is_touch_sndstatus())
			return true;
		break;
	case FEEDBACK_PATTERN_KEY0 ... FEEDBACK_PATTERN_KEY_BACK:
		if (CHECK_SOUND(type) && !is_keytone_sndstatus())
			return true;
		else if (CHECK_VIBRATION(type) && !feedbackstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_LOCK:
	case FEEDBACK_PATTERN_UNLOCK:
	case FEEDBACK_PATTERN_MOBILE_LOCK_SWIPE:
	case FEEDBACK_PATTERN_MOBILE_UNLOCK_SWIPE:
		if (CHECK_SOUND(type) && !lock_sndstatus)
			return true;
		break;
	default:
		break;
	}
	return false;
}

static int mobile_get_strength_type(int type, int pattern)
{
	if (CHECK_SOUND(type)) {
		if (pattern == FEEDBACK_PATTERN_TAP)
			return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_TOUCH;
		else if (pattern >= FEEDBACK_PATTERN_KEY0 && pattern <= FEEDBACK_PATTERN_KEY_BACK)
			return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_DIALER;
		else if (pattern == FEEDBACK_PATTERN_VOLUME_KEY)
			return VOLUME_TYPE_RINGTONE;
		else if (camerastatus && shutter_sndstatus && pattern == FEEDBACK_PATTERN_MOBILE_SCREEN_CAPTURE)
			return VOLUME_TYPE_FIXED;
		else
			return VOLUME_TYPE_SYSTEM;
	} else if (CHECK_VIBRATION(type)) {
		if (pattern >= FEEDBACK_PATTERN_MESSAGE &&
		    pattern <= FEEDBACK_PATTERN_GENERAL)
			return noti_level * HAPTIC_FEEDBACK_STEP;
		else
			return vib_level * HAPTIC_FEEDBACK_STEP;
	}

	return -EINVAL;
}

static const struct profile_ops mobile_profile_ops = {
	.profile               = "mobile",
	.init                  = mobile_init,
	.exit                  = mobile_exit,
	.get_switched_pattern  = mobile_get_switched_pattern,
	.get_always_alert_case = mobile_get_always_alert_case,
	.get_always_off_case   = mobile_get_always_off_case,
	.get_strength_type     = mobile_get_strength_type,
	.max_type              = FEEDBACK_TYPE_MOBILE_END,
	.max_pattern           = FEEDBACK_PATTERN_MOBILE_END,
	.str_type              = mobile_str_type,
	.str_pattern           = mobile_str_pattern,
};

PROFILE_OPS_REGISTER(&mobile_profile_ops);
