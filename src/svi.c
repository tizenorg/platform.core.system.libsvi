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

#include "svi.h"
#include "feedback.h"
#include "svi-log.h"

#define SVI_TEMP_HANDLE		1

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

feedback_pattern_e feedback_sound[] = {
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : GENERAL */
	FEEDBACK_PATTERN_TAP,
	FEEDBACK_PATTERN_TAP,
	FEEDBACK_PATTERN_TAP,
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : TEXT_NUMERIC_INPUT */
	FEEDBACK_PATTERN_SIP,
	FEEDBACK_PATTERN_SIP_BACKSPACE,
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : DAILER */
	FEEDBACK_PATTERN_KEY0,
	FEEDBACK_PATTERN_KEY1,
	FEEDBACK_PATTERN_KEY2,
	FEEDBACK_PATTERN_KEY3,
	FEEDBACK_PATTERN_KEY4,
	FEEDBACK_PATTERN_KEY5,
	FEEDBACK_PATTERN_KEY6,
	FEEDBACK_PATTERN_KEY7,
	FEEDBACK_PATTERN_KEY8,
	FEEDBACK_PATTERN_KEY9,
	FEEDBACK_PATTERN_KEY_STAR,
	FEEDBACK_PATTERN_KEY_SHARP,
	/* TOUCH : H/W OR SOFT TOUCH : HOLD(TAP & HOLD) */
	FEEDBACK_PATTERN_HOLD,
	/* TOUCH : H/W OR SOFT TOUCH : MULTI TAP */
	FEEDBACK_PATTERN_MULTI_TAP,
	/* TOUCH : H/W OR SOFT TOUCH : TAP */
	FEEDBACK_PATTERN_HW_TAP,
	/* TOUCH : H/W OR SOFT TOUCH : TAP & HOLD */
	FEEDBACK_PATTERN_HW_HOLD,

	/* OPERATION : POWER ON/OFF */
	FEEDBACK_PATTERN_POWERON,
	FEEDBACK_PATTERN_POWEROFF,
	/* OPERATION : CHARGECONN */
	FEEDBACK_PATTERN_CHARGERCONN,
	/* OPERATION : FULLCHAREGED */
	FEEDBACK_PATTERN_FULLCHARGED,
	/* OPERATION : LOW BATTERY */
	FEEDBACK_PATTERN_LOWBATT,
	/* OPERATION : LOCK/UNLOCK */
	FEEDBACK_PATTERN_LOCK,
	FEEDBACK_PATTERN_UNLOCK,
	/* OPERATION : CALL CONNECT/ DISCONNECT */
	FEEDBACK_PATTERN_CALLCONNECT,
	FEEDBACK_PATTERN_DISCALLCONNECT,
	/* OPERATION : MINUTE MINDER */
	FEEDBACK_PATTERN_MINUTEMINDER,
	/* OPERATION : VIBRATION */
	FEEDBACK_PATTERN_VIBRATION,
	/* OPERATION : BALLOON MESSAGE SEND/RECV */
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	/* OPERATION : ON/OFF SLIDER */
	FEEDBACK_PATTERN_NONE,
	/* OPERATION : CAMERA SHUTTER / SCREEN CAPTURE */
	FEEDBACK_PATTERN_SHUTTER,
	/* OPERATION : LIST RE-ORDER */
	FEEDBACK_PATTERN_LIST_REORDER,
	/* OPERATION : LIST SLIDER */
	FEEDBACK_PATTERN_SLIDER_SWEEP,
};

feedback_pattern_e feedback_vibration[] = {
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : GENERAL */
	FEEDBACK_PATTERN_TAP,
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : TEXT_NUMERIC_INPUT */
	FEEDBACK_PATTERN_SIP,
	FEEDBACK_PATTERN_SIP_BACKSPACE,
	/* TOUCH : SCREEN TOUCH : TAP(TOUCH & RELEASE) : DAILER */
	FEEDBACK_PATTERN_KEY0,
	FEEDBACK_PATTERN_KEY1,
	FEEDBACK_PATTERN_KEY2,
	FEEDBACK_PATTERN_KEY3,
	FEEDBACK_PATTERN_KEY4,
	FEEDBACK_PATTERN_KEY5,
	FEEDBACK_PATTERN_KEY6,
	FEEDBACK_PATTERN_KEY7,
	FEEDBACK_PATTERN_KEY8,
	FEEDBACK_PATTERN_KEY9,
	FEEDBACK_PATTERN_KEY_STAR,
	FEEDBACK_PATTERN_KEY_SHARP,
	/* TOUCH : H/W OR SOFT TOUCH : HOLD(TAP & HOLD) */
	FEEDBACK_PATTERN_HOLD,
	/* TOUCH : H/W OR SOFT TOUCH : MULTI TAP */
	FEEDBACK_PATTERN_MULTI_TAP,
	/* TOUCH : H/W OR SOFT TOUCH : TAP */
	FEEDBACK_PATTERN_HW_TAP,
	/* TOUCH : H/W OR SOFT TOUCH : TAP & HOLD */
	FEEDBACK_PATTERN_HW_HOLD,

	/* NOTIFICATION : INCOMING : CALL */
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	/* NOTIFICATION : INCOMING : MESSAGE */
	FEEDBACK_PATTERN_MESSAGE,
	/* NOTIFICATION : INCOMING : EMAIL */
	FEEDBACK_PATTERN_EMAIL,
	/* NOTIFICATION : ALARM : WAKEUP */
	FEEDBACK_PATTERN_WAKEUP,
	/* NOTIFICATION : ALARM : SCHEDULE */
	FEEDBACK_PATTERN_SCHEDULE,
	/* NOTIFICATION : ALARM : TIMER */
	FEEDBACK_PATTERN_TIMER,
	/* NOTIFICATION : GENERAL(TICKER/IM/SMS ETC) */
	FEEDBACK_PATTERN_GENERAL,

	/* OPERATION : POWER ON/OFF */
	FEEDBACK_PATTERN_POWERON,
	FEEDBACK_PATTERN_POWEROFF,
	/* OPERATION : CHARGECONN */
	FEEDBACK_PATTERN_CHARGERCONN,
	/* OPERATION : FULLCHAREGED */
	FEEDBACK_PATTERN_FULLCHARGED,
	/* OPERATION : LOW BATTERY */
	FEEDBACK_PATTERN_LOWBATT,
	/* OPERATION : LOCK/UNLOCK */
	FEEDBACK_PATTERN_LOCK,
	FEEDBACK_PATTERN_UNLOCK,
	/* OPERATION : CALL CONNECT/ DISCONNECT */
	FEEDBACK_PATTERN_CALLCONNECT,
	FEEDBACK_PATTERN_DISCALLCONNECT,
	/* OPERATION : MINUTE MINDER */
	FEEDBACK_PATTERN_MINUTEMINDER,
	/* OPERATION : VIBRATION */
	FEEDBACK_PATTERN_VIBRATION,
	/* OPERATION : BALLOON MESSAGE SEND/RECV */
	FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_NONE,
	/* OPERATION : ON/OFF SLIDER */
	FEEDBACK_PATTERN_NONE,
	/* OPERATION : CAMERA SHUTTER / SCREEN CAPTURE */
	FEEDBACK_PATTERN_SHUTTER,
	/* OPERATION : LIST RE-ORDER */
	FEEDBACK_PATTERN_LIST_REORDER,
	/* OPERATION : LIST SLIDER */
	FEEDBACK_PATTERN_SLIDER_SWEEP,
};

API int svi_init(int *handle)
{
	int ret = -1;

	if (handle == NULL) {
		SVILOG("ERROR!! Invalid parameter : handle(NULL)");
		return SVI_ERROR;
	}

	ret = feedback_initialize();
	if (FEEDBACK_FAILED(ret)) {
		SVILOG("ERROR!! feedback_initialize is failed");
		return SVI_ERROR;
	}

	*handle = SVI_TEMP_HANDLE;
	return SVI_SUCCESS;
}

API int svi_fini(int handle)
{
	int ret = -1;

	if (handle != SVI_TEMP_HANDLE) {
		SVILOG("ERROR!! Invalid parameter : handle(%d)", handle);
		return SVI_ERROR;
	}

	ret = feedback_deinitialize();
	if (FEEDBACK_FAILED(ret)) {
		SVILOG("ERROR!! feedback_deinitialize is failed");
		return SVI_ERROR;
	}

	return SVI_SUCCESS;
}

API int svi_play_sound(int handle, sound_type sound_key)
{
	int ret = -1;
	feedback_pattern_e pattern = FEEDBACK_PATTERN_NONE;

	if (handle != SVI_TEMP_HANDLE) {
		SVILOG("ERROR!! Invalid parameter : handle(%d)", handle);
		return SVI_ERROR;
	}

	if (sound_key < SVI_SND_NONE || sound_key >= SVI_SND_ENUM_END) {
		SVILOG("ERROR!! Invalid parameter : sound_key(%d)", sound_key);
		return SVI_ERROR;
	}

	if (sound_key == SVI_SND_NONE) {
		pattern = FEEDBACK_PATTERN_NONE;
		SVILOG("pattern is NONE");
	} else {
		pattern = feedback_sound[sound_key];
		SVILOG("sound_key : %d, pattern : %d", sound_key, pattern);
	}

	ret = feedback_play_type(FEEDBACK_TYPE_SOUND, pattern);
	if (FEEDBACK_FAILED(ret)) {
		SVILOG("ERROR!! feedback_play_type is failed");
		return SVI_ERROR;
	}

	return SVI_SUCCESS;
}

API int svi_play_vib(int handle, vibration_type vibration_key)
{
	int ret = -1;
	feedback_pattern_e pattern = FEEDBACK_PATTERN_NONE;

	if (handle != SVI_TEMP_HANDLE) {
		SVILOG("ERROR!! Invalid parameter : handle(%d)", handle);
		return SVI_ERROR;
	}

	if (vibration_key < SVI_VIB_NONE || vibration_key >= SVI_VIB_ENUM_END) {
		SVILOG("ERROR!! Invalid parameter : sound_key(%d)", vibration_key);
		return SVI_ERROR;
	}

	if (vibration_key == SVI_VIB_NONE) {
		pattern = FEEDBACK_PATTERN_NONE;
		SVILOG("pattern is NONE");
	} else {
		pattern = feedback_vibration[vibration_key];
		SVILOG("vibration_key : %d, pattern : %d", vibration_key, pattern);
	}

	ret = feedback_play_type(FEEDBACK_TYPE_VIBRATION, pattern);
	if (FEEDBACK_FAILED(ret)) {
		SVILOG("ERROR!! feedback_play_type is failed");
		return SVI_ERROR;
	}

	return SVI_SUCCESS;
}

API int svi_play(int handle, vibration_type vibration_key, sound_type sound_key)
{
	int ret_snd = svi_play_sound(handle, sound_key);
	int ret_vib = svi_play_vib(handle, vibration_key);

	if (ret_snd == SVI_ERROR || ret_vib == SVI_ERROR) {
		return SVI_ERROR;
	} else {
		return SVI_SUCCESS;
	}
}

API int svi_set_path(int svi_type, int svi_enum, char* path)
{
	SVILOG("This api is not supported");
	return SVI_ERROR;
}

API int svi_get_path(int svi_type, int svi_enum, char* buf, unsigned int bufLen)
{
	int ret = -1;
	feedback_pattern_e pattern = FEEDBACK_PATTERN_NONE;
	char *path = NULL;

	if (svi_type <= SVI_TYPE_NONE || svi_type >= SVI_TYPE_END) {
		SVILOG("ERROR!! Invalid parameter : svi_type(%d).", svi_type);
		return SVI_ERROR;
	}

	if (svi_type == SVI_TYPE_SND) {
		if (svi_enum <= SVI_SND_NONE || svi_enum >= SVI_SND_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", svi_enum);
			return SVI_ERROR;
		}

		pattern = feedback_sound[svi_enum];
		ret = feedback_get_resource_path(FEEDBACK_TYPE_SOUND, pattern, &path);
	} else if (svi_type == SVI_TYPE_VIB) {
		if (svi_enum <= SVI_VIB_NONE || svi_enum >= SVI_VIB_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", svi_enum);
			return SVI_ERROR;
		}

		pattern = feedback_vibration[svi_enum];
		ret = feedback_get_resource_path(FEEDBACK_TYPE_VIBRATION, pattern, &path);
	}

	if (FEEDBACK_FAILED(ret)) {
		SVILOG("ERROR!! feedback_play_type is failed");
		return SVI_ERROR;
	}

	snprintf(buf, bufLen, "%s", path);
	free(path);

	return SVI_SUCCESS;
}
