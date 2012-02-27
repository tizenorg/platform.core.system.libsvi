/*
 *  libsvi
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seokkyu Jang <seokkyu.jang@samsung.com>
 * Contact: Sangil Yoon <si83.yoon@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <mm_sound_private.h>
#include <vconf.h>
#include <devman_haptic.h>

#include <svi.h>
#include "svi-log.h"

#define SVI_DATA_DIR "/usr/share/svi"
#define SVI_SOUND_DIR SVI_DATA_DIR"/sound"
#define SVI_HAPTIC_DIR SVI_DATA_DIR"/haptic"

#define SOUND_TOUCH_DIR SVI_SOUND_DIR"/touch"
#define HAPTIC_TOUCH_DIR SVI_HAPTIC_DIR"/touch"

#define HAPTIC_NOTI_DIR SVI_HAPTIC_DIR"/notification"

#define SOUND_OPER_DIR SVI_SOUND_DIR"/operation"
#define HAPTIC_OPER_DIR SVI_HAPTIC_DIR"/operation"


#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

//#define PERFORM_CHECK

#ifdef PERFORM_CHECK
static long long ms = 0;

#define MICROSECONDS(tv)        ((tv.tv_sec * 1000000ll) + tv.tv_usec)

#define ESTIMATE_PERFORMANCE() \
	do { \
		struct timeval tv; \
		if (ms == 0) { \
			gettimeofday(&tv, NULL); \
			ms = MICROSECONDS(tv); \
			fprintf(stderr, "%s start time : %lld\n", __func__, ms); \
		} else { \
			gettimeofday(&tv, NULL); \
			fprintf(stderr, "%s elapsed time : %lld\n", __func__, MICROSECONDS(tv) - ms); \
			ms = 0; \
		} \
	} while(0)
#else
#define ESTIMATE_PERFORMANCE()
#endif

const char* snd_file[] = {
	SOUND_TOUCH_DIR"/touch1.wav",
	SOUND_TOUCH_DIR"/touch2.wav",
	SOUND_TOUCH_DIR"/touch3.wav",
	SOUND_TOUCH_DIR"/sip.wav",
	SOUND_TOUCH_DIR"/hold.wav",
	SOUND_TOUCH_DIR"/multi_tap.wav",
	SOUND_TOUCH_DIR"/hw_tap.wav",
	SOUND_TOUCH_DIR"/hw_hold.wav",

	SOUND_TOUCH_DIR"/key0.wav",
	SOUND_TOUCH_DIR"/key1.wav",
	SOUND_TOUCH_DIR"/key2.wav",
	SOUND_TOUCH_DIR"/key3.wav",
	SOUND_TOUCH_DIR"/key4.wav",
	SOUND_TOUCH_DIR"/key5.wav",
	SOUND_TOUCH_DIR"/key6.wav",
	SOUND_TOUCH_DIR"/key7.wav",
	SOUND_TOUCH_DIR"/key8.wav",
	SOUND_TOUCH_DIR"/key9.wav",
	SOUND_TOUCH_DIR"/keyasterisk.wav",
	SOUND_TOUCH_DIR"/keysharp.wav",

	SOUND_OPER_DIR"/power_on.wav",
	SOUND_OPER_DIR"/power_off.wav",
	SOUND_OPER_DIR"/charger_connection.wav",
	SOUND_OPER_DIR"/fully_charged.wav",
	SOUND_OPER_DIR"/low_battery.wav",
	SOUND_OPER_DIR"/lock.wav",
	SOUND_OPER_DIR"/unlock.wav",
	SOUND_OPER_DIR"/call_connect.wav",
	SOUND_OPER_DIR"/call_disconnect.wav",
	SOUND_OPER_DIR"/minute_minder.wav",
	SOUND_OPER_DIR"/vibration.wav",
	SOUND_OPER_DIR"/new_chat.wav",
	SOUND_OPER_DIR"/sent_chat.wav",
	SOUND_OPER_DIR"/on_off_slider.wav",
	SOUND_OPER_DIR"/shutter.wav",
};

const char* haptic_file[] = {
	HAPTIC_TOUCH_DIR"/touch.ivt",
	HAPTIC_TOUCH_DIR"/sip.ivt",
	HAPTIC_TOUCH_DIR"/hold.ivt",
	HAPTIC_TOUCH_DIR"/multi_tap.ivt",
	HAPTIC_TOUCH_DIR"/hw_tap.ivt",
	HAPTIC_TOUCH_DIR"/hw_hold.ivt",

	HAPTIC_TOUCH_DIR"/key0.ivt",
	HAPTIC_TOUCH_DIR"/key1.ivt",
	HAPTIC_TOUCH_DIR"/key2.ivt",
	HAPTIC_TOUCH_DIR"/key3.ivt",
	HAPTIC_TOUCH_DIR"/key4.ivt",
	HAPTIC_TOUCH_DIR"/key5.ivt",
	HAPTIC_TOUCH_DIR"/key6.ivt",
	HAPTIC_TOUCH_DIR"/key7.ivt",
	HAPTIC_TOUCH_DIR"/key8.ivt",
	HAPTIC_TOUCH_DIR"/key9.ivt",
	HAPTIC_TOUCH_DIR"/keyasterisk.ivt",
	HAPTIC_TOUCH_DIR"/keysharp.ivt",

	HAPTIC_NOTI_DIR"/incoming_call1.ivt",
	HAPTIC_NOTI_DIR"/incoming_call2.ivt",
	HAPTIC_NOTI_DIR"/incoming_call3.ivt",
	HAPTIC_NOTI_DIR"/message.ivt",
	HAPTIC_NOTI_DIR"/email.ivt",
	HAPTIC_NOTI_DIR"/wakeup.ivt",
	HAPTIC_NOTI_DIR"/schedule.ivt",
	HAPTIC_NOTI_DIR"/timer.ivt",
	HAPTIC_NOTI_DIR"/general.ivt",

	HAPTIC_OPER_DIR"/power_on.ivt",
	HAPTIC_OPER_DIR"/power_off.ivt",
	HAPTIC_OPER_DIR"/charger_connection.ivt",
	HAPTIC_OPER_DIR"/fully_charged.ivt",
	HAPTIC_OPER_DIR"/low_battery.ivt",
	HAPTIC_OPER_DIR"/lock.ivt",
	HAPTIC_OPER_DIR"/unlock.ivt",
	HAPTIC_OPER_DIR"/call_connect.ivt",
	HAPTIC_OPER_DIR"/call_disconnect.ivt",
	HAPTIC_OPER_DIR"/minute_minder.ivt",
	HAPTIC_OPER_DIR"/vibration.ivt",
	HAPTIC_OPER_DIR"/new_chat.ivt",
	HAPTIC_OPER_DIR"/sent_chat.ivt",
	HAPTIC_OPER_DIR"/on_off_slider.ivt",
	HAPTIC_OPER_DIR"/shutter.ivt",
};

static int soundon = -1;
static int vib_level = -1;
static int sndstatus = -1;
static int vibstatus = -1;

void soundon_cb(keynode_t *key, void* data)
{
	soundon = vconf_keynode_get_bool(key);
	SVILOG("[[[[[[[[[[[[[[soundon changed!! new soundon => %d\n", soundon);
	return;
}

void vib_cb(keynode_t *key, void* data)
{
	vib_level = vconf_keynode_get_int(key);

	SVILOG("[[[[[[[[[[[[[[vib_level changed!! new vib_level => %d\n", vib_level);

	return;
}

void sndstatus_cb(keynode_t *key, void* data)
{
	sndstatus = vconf_keynode_get_int(key);

	SVILOG("[[[[[[[[[[[[[[sndstatus changed!! new sndstatus => %d\n", sndstatus);

	return;
}

void vibstatus_cb(keynode_t *key, void* data)
{
	vibstatus = vconf_keynode_get_bool(key);

	SVILOG("[[[[[[[[[[[[[[vibstatus changed!! new vibstatus => %d\n", vibstatus);

	return;
}


volume_type_t get_volume_type(sound_type sound_key)
{
	volume_type_t type = VOLUME_TYPE_SYSTEM;

	if (sound_key >= SVI_SND_OPERATION_POWERON && sound_key <= SVI_SND_OPERATION_SCRCAPTURE)
		type = VOLUME_TYPE_SYSTEM;
	else
		type = VOLUME_TYPE_SYSTEM;

	return type;
}

API int svi_init(int *handle)
{
	int ret_vib = SVI_SUCCESS;
	int v_handle = 0;

	/* Sound Init */
	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &soundon) < 0)
		SVILOG("vconf_get_int(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &soundon) ==> FAIL!!\n");
	SVILOG("vconf_get_int(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &soundon) ==> %d\n", soundon);

	if (vconf_get_int("memory/Sound/SoundStatus", &sndstatus) < 0)
		SVILOG("vconf_get_int(memory/Sound/SoundStatus, &sndstatus) ==> FAIL!!\n");
	SVILOG("vconf_get_int(memory/Sound/SoundStatus, &sndstatus) ==> %d\n", sndstatus);

	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, soundon_cb, NULL);
	vconf_notify_key_changed("memory/Sound/SoundStatus", sndstatus_cb, NULL);

	/* Vibration Init */
	v_handle = device_haptic_open( DEV_IDX_0 , 0x01); // new haptic lib.

	if (v_handle < 0)
	{
		SVILOG("device_haptic_open(DEV_IDX_0) ==> FAIL!!\n");
		ret_vib = SVI_ERROR;
	}
	else
	{
		if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0)    //check vibration status
			SVILOG("vconf_get_int(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> FAIL!!\n");
		SVILOG("vconf_get_int(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> %d\n", vibstatus);

		if (vconf_get_int(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) < 0)    //check vib_level
			SVILOG("vconf_get_int(VCONFKEY_SETAPPL_VIB_FEEDBACK_INT, &vib_level) ==> FAIL!!\n");
		SVILOG("vconf_get_int(VCONFKEY_SETAPPL_VIB_FEEDBACK_INT, &vib_level) ==> %d\n", vib_level);

		vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, vibstatus_cb, NULL);
		vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, vib_cb, NULL);
		ret_vib = SVI_SUCCESS;
	}

	if (ret_vib == SVI_ERROR) {
		vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, soundon_cb);
		vconf_ignore_key_changed("memory/Sound/SoundStatus", sndstatus_cb);
		return SVI_ERROR;
	} else {
		*handle = v_handle;
		return SVI_SUCCESS;
	}
}

API int svi_fini(int handle)
{
	int ret_vib = SVI_SUCCESS;

	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, soundon_cb);
	vconf_ignore_key_changed("memory/Sound/SoundStatus", sndstatus_cb);

	if (handle > 0) {
		vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, vibstatus_cb);
		vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, vib_cb);
		if (device_haptic_close(handle) < 0) {
			ret_vib = SVI_ERROR;
		} else {
			ret_vib = SVI_SUCCESS;
		}
	}

	if ( ret_vib == SVI_ERROR) {
		return SVI_ERROR;
	} else {
		return SVI_SUCCESS;
	}
}

API int svi_play_sound(int handle, sound_type sound_key)
{
	int ret_snd = SVI_SUCCESS;
	struct stat buf;
	SVILOG("sound_key = %d\n", sound_key);

	if (handle < 0) {
		SVILOG("ERROR!! Please call svi_init() for sound init \n");
		ret_snd = SVI_ERROR;
	}
	else
	{
		if (sound_key > SVI_SND_NONE && sound_key <= SVI_SND_OPERATION_SCRCAPTURE)
		{
			SVILOG("soundon = %d, sndstatus = %d \n", soundon, sndstatus);
			if (soundon != 0 && sndstatus == 0)
			{
				if(stat(snd_file[sound_key], &buf)) { /*check file existence*/
					SVILOG("ERROR!! %s is not presents\n", snd_file[sound_key]);
					return SVI_ERROR;
				}
				if (mm_sound_play_keysound(snd_file[sound_key], get_volume_type(sound_key)) < 0)
				{
					SVILOG("ERROR!! mm_sound_play_keysound() returned error.\n");
					ret_snd = SVI_ERROR;
				}
				SVILOG("SND filename is %s\n", snd_file[sound_key]);
			}
		}
		else if (sound_key != SVI_SND_NONE) {
			ret_snd = SVI_ERROR;
		}
	}

	return ret_snd;
}

API int svi_play_vib(int handle, vibration_type vibration_key)
{
	int ret_vib = SVI_SUCCESS;
	struct stat buf;

	SVILOG("vibration key - %d\n", vibration_key);

	if (handle < 0) {
		SVILOG("ERROR!! Please call svi_init() for vibration init \n");
		ret_vib = SVI_ERROR;
	} else 	{
		if (vibration_key > SVI_VIB_NONE && vibration_key <= SVI_VIB_OPERATION_SHUTTER) {
			if (vibration_key == SVI_VIB_OPERATION_FULLYCHARGED || vibration_key == SVI_VIB_OPERATION_LOTBATT) {
				SVILOG("vibration type is SVI_VIB_OPERATION_FULLYCHARGED or SVI_VIB_OPERATION_LOTBATT\n");
				if (device_haptic_play_file(handle, haptic_file[vibration_key], 1, 5) < 0) {
					SVILOG("ERROR!! device_haptic_play_file() returned error.\n");
					ret_vib = SVI_ERROR;
				}
				SVILOG("device_haptic_play_file(handle, vibration_key, 1, 5)\n");
			} else {
				SVILOG("vibstatus = %d, vib_level = %d\n", vibstatus, vib_level);
				if (vibstatus != 0) {
					if(stat(haptic_file[vibration_key], &buf)) { /*check file existence*/
						SVILOG("ERROR!! %s is not presents\n", haptic_file[vibration_key]);
						return SVI_ERROR;
					}
					if (device_haptic_play_file(handle, haptic_file[vibration_key], 1, vib_level) < 0) {
						SVILOG("ERROR!! device_haptic_play_file() returned error.\n");
						ret_vib = SVI_ERROR;
					}
					SVILOG("device_haptic_play_file(handle, vibration_key, 1, %d)\n", vib_level);
				}
			}
		}
		else if (vibration_key != SVI_VIB_NONE) {
			ret_vib = SVI_ERROR;
		}
	}

	return ret_vib;

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

API int svi_get_path(int svi_type, int svi_enum, char* buf, unsigned int bufLen)
{
	vibration_type vib_enum_type = SVI_VIB_NONE;
	sound_type snd_enum_type = SVI_SND_NONE;
	unsigned int pathLen = 0;

	SVILOG("starts\n");

	if (buf == NULL || bufLen <= 0) {
		SVILOG("ERROR!! invalid input parameters.\n");
		return SVI_ERROR;
	}

	if (!(svi_type == SVI_TYPE_SND || svi_type == SVI_TYPE_VIB)) {
		SVILOG("ERROR!! invalid svi_type(%d).\n", svi_type);
		return SVI_ERROR;
	}


	if (svi_type == SVI_TYPE_SND) {
		snd_enum_type = (sound_type)svi_enum;

		if (snd_enum_type < SVI_SND_TOUCH_TOUCH1 || snd_enum_type> SVI_SND_OPERATION_SCRCAPTURE) {
			SVILOG("ERROR! invalid svi_enum(%d)\n", snd_enum_type);
			return SVI_ERROR;
		}
		pathLen = strlen(snd_file[snd_enum_type]);
		if (pathLen <= 0) {
			SVILOG("ERROR!! NULL path.\n");
			return SVI_ERROR;
		}
		if (pathLen < bufLen) {
			strncpy(buf, snd_file[snd_enum_type], bufLen);
		} else {
			SVILOG("ERROR!! Overflow.\n");
			return SVI_ERROR;
		}
	} else if (svi_type == SVI_TYPE_VIB) {
		vib_enum_type = (vibration_type) svi_enum;
		if (vib_enum_type < SVI_VIB_TOUCH_TOUCH || vib_enum_type > SVI_VIB_OPERATION_SHUTTER) {
			SVILOG("ERROR! invalid svi_enum(%d)\n", vib_enum_type);
			return SVI_ERROR;
		}
		pathLen = strlen(haptic_file[vib_enum_type]);
		if (pathLen <= 0) {
			SVILOG("ERROR!! NULL path.\n");
			return SVI_ERROR;
		}
		if (pathLen < bufLen) {
			strncpy(buf, haptic_file[vib_enum_type], bufLen);
		} else {
			SVILOG("ERROR!! Overflow.\n");
			return SVI_ERROR;
		}
	}

	SVILOG("ends\n");
	return SVI_SUCCESS;
}

