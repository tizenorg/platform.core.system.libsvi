/*
 *  libsvi
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Hyungdeuk Kim <hd3.kim@samsung.com>
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

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <mm_sound_private.h>
#include <vconf.h>
#include <devman_haptic.h>

#include <fcntl.h>
#include <errno.h>

#include "svi.h"
#include "svi-log.h"
#include "svi-file.h"

#define SVI_RETRY_CNT 1
#define MAX_FILE_PATH 512

#ifndef API
#define API __attribute__ ((visibility("default")))
#endif

#ifdef PERFORM_CHECK
static long long ms = 0;

#define MICROSECONDS(tv)        ((tv.tv_sec * 1000000ll) + tv.tv_usec)

#define ESTIMATE_PERFORMANCE() \
	do { \
		struct timeval tv; \
		if (ms == 0) { \
			gettimeofday(&tv, NULL); \
			ms = MICROSECONDS(tv); \
			fprintf(stderr, "%s start time : %lld", __func__, ms); \
		} else { \
			gettimeofday(&tv, NULL); \
			fprintf(stderr, "%s elapsed time : %lld", __func__, MICROSECONDS(tv) - ms); \
			ms = 0; \
		} \
	} while(0)
#else
#define ESTIMATE_PERFORMANCE()
#endif

static int soundon = -1;
static int vib_level = -1;
static int sndstatus = -1;
static int vibstatus = -1;

static void __svi_soundon_cb(keynode_t *key, void* data)
{
	soundon = vconf_keynode_get_bool(key);
	SVILOG("[[[[[[[[[[[[[[soundon changed!! new soundon => %d", soundon);
	return;
}

static void __svi_vib_cb(keynode_t *key, void* data)
{
	vib_level = vconf_keynode_get_int(key);

	SVILOG("[[[[[[[[[[[[[[vib_level changed!! new vib_level => %d", vib_level);

	return;
}

static void __svi_sndstatus_cb(keynode_t *key, void* data)
{
	sndstatus = vconf_keynode_get_int(key);

	SVILOG("[[[[[[[[[[[[[[sndstatus changed!! new sndstatus => %d", sndstatus);

	return;
}

static void __svi_vibstatus_cb(keynode_t *key, void* data)
{
	vibstatus = vconf_keynode_get_bool(key);

	SVILOG("[[[[[[[[[[[[[[vibstatus changed!! new vibstatus => %d", vibstatus);

	return;
}

static volume_type_t __svi_get_volume_type(sound_type sound_key)
{
	volume_type_t type = VOLUME_TYPE_SYSTEM;

	if (sound_key >= SVI_SND_OPERATION_POWERON && sound_key <= SVI_SND_OPERATION_SCRCAPTURE)
		type = VOLUME_TYPE_SYSTEM;
	else
		type = VOLUME_TYPE_SYSTEM;

	return type;
}

static int __svi_restore_default_file(int svi_type, int svi_enum)
{
	vibration_type vib_enum_type = SVI_VIB_NONE;
	sound_type snd_enum_type = SVI_SND_NONE;
	const char* cur_path = NULL;
	char default_path[MAX_FILE_PATH] = {0,};
	char *temp = NULL;
	struct stat buf;

	if(svi_type <=SVI_TYPE_NONE || svi_type >= SVI_TYPE_END) {
		SVILOG("ERROR!! invalid svi_type(%d).", svi_type);
		return SVI_ERROR;
	}

	if (svi_type == SVI_TYPE_SND) {
		snd_enum_type = (sound_type)svi_enum;

		if (snd_enum_type <= SVI_SND_NONE || snd_enum_type >= SVI_SND_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", snd_enum_type);
			return SVI_ERROR;
		}

		cur_path = snd_file[snd_enum_type];
	} else {
		vib_enum_type = (vibration_type)svi_enum;

		if (vib_enum_type <= SVI_VIB_NONE || vib_enum_type >= SVI_VIB_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", vib_enum_type);
			return SVI_ERROR;
		}

		cur_path = haptic_file[vib_enum_type];
	}

	if( (cur_path == NULL) || (strlen(cur_path) == 0) ) {
		SVILOG("ERROR! current path is invalid");
		return SVI_ERROR;
	}

	temp = strcat(default_path, SVI_ORIGIN_DATA_DIR);

	strcat(temp, cur_path+strlen(SVI_DATA_DIR));

	SVILOG("default_path : %s", default_path);

	if(stat(default_path, &buf)) { /*check file existence*/
		SVILOG("ERROR!! default file for type(%d),enum(%d) is not presents", svi_type, svi_enum);
		return SVI_ERROR;
	}

	if(unlink(cur_path) < 0) {
		SVILOG("WARNING!! unlink(%s) error(%d)", cur_path, errno);
	}

	if(symlink(default_path, cur_path) < 0) {
		SVILOG("ERROR!! symlink(%s) error(%d)", default_path, errno);
		return SVI_ERROR;
	}

	return SVI_SUCCESS;
}

API int svi_init(int *handle)
{
	int v_handle = 0;

	/* Sound Init */
	if (vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &soundon) < 0) {
		SVILOG("vconf_get_bool(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &soundon) ==> FAIL!!");
		return SVI_ERROR;
	} else {
		SVILOG("vconf_get_int(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, &soundon) ==> %d", soundon);
	}

	if (vconf_get_int(VCONFKEY_SOUND_STATUS, &sndstatus) < 0) {
		SVILOG("vconf_get_int(VCONFKEY_SOUND_STATUS, &sndstatus) ==> FAIL!!");
		return SVI_ERROR;
	} else {
		SVILOG("vconf_get_int(VCONFKEY_SOUND_STATUS, &sndstatus) ==> %d", sndstatus);
	}

	/* Vibration Init */
	v_handle = device_haptic_open( DEV_IDX_0 , 0x01); // new haptic lib.
	if (v_handle < 0) {
		SVILOG("device_haptic_open(DEV_IDX_0) ==> FAIL!!");
		return SVI_ERROR;
	} else {
		/* check vibration status */
		if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0) {
			SVILOG("vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> FAIL!!");
			return SVI_ERROR;
		} else {
			SVILOG("vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) ==> %d", vibstatus);
		}

		/* check vib_level */
		if (vconf_get_int(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) < 0) {
			SVILOG("vconf_get_int(VCONFKEY_SETAPPL_VIB_FEEDBACK_INT, &vib_level) ==> FAIL!!");
			return SVI_ERROR;
		} else {
			SVILOG("vconf_get_int(VCONFKEY_SETAPPL_VIB_FEEDBACK_INT, &vib_level) ==> %d", vib_level);
		}

		*handle = v_handle;
	}

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, __svi_soundon_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SOUND_STATUS, __svi_sndstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, __svi_vibstatus_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, __svi_vib_cb, NULL);

	return SVI_SUCCESS;
}

API int svi_fini(int handle)
{
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_SOUND_STATUS_BOOL, __svi_soundon_cb);
	vconf_ignore_key_changed(VCONFKEY_SOUND_STATUS, __svi_sndstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, __svi_vibstatus_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, __svi_vib_cb);

	if (handle > 0) {
		if (device_haptic_close(handle) < 0) {
			return SVI_ERROR;
		} else {
			return SVI_SUCCESS;
		}
	}

	return SVI_ERROR;
}

API int svi_play_sound(int handle, sound_type sound_key)
{
	int ret_snd = SVI_SUCCESS;
	int ret = 0;
	int retry = 1;

	if (handle < 0) {
		SVILOG("ERROR!! Please call svi_init() for sound init ");
		ret_snd = SVI_ERROR;
	} else {
		if (sound_key > SVI_SND_NONE && sound_key <= SVI_SND_OPERATION_SCRCAPTURE) {
			SVILOG("sound_key = %d, soundon = %d, sndstatus = %d ", sound_key, soundon, sndstatus);
			if (soundon != 0 && sndstatus == 0) {
				do {
					ret = mm_sound_play_keysound(snd_file[sound_key], __svi_get_volume_type(sound_key));
					if(ret == MM_ERROR_NONE) {
						SVILOG("Play success! SND filename is %s", snd_file[sound_key]);
						break;
					} else {
						if(ret == MM_ERROR_SOUND_FILE_NOT_FOUND) {
							SVILOG("mm_sound_play_keysound MM_ERROR_SOUND_FILE_NOT_FOUND error");
							if(__svi_restore_default_file(SVI_TYPE_SND, sound_key) == SVI_ERROR) {
								SVILOG("ERROR!! __svi_restore_origin_file(%d/%d) error", SVI_TYPE_SND, sound_key);
								ret_snd = SVI_ERROR;
								break;
							} else {
								SVILOG("sound file link is restored. sound play will be retried. ");
							}
						} else {
							SVILOG("ERROR!! mm_sound_play_keysound() returned error(%d)", ret);
							ret_snd = SVI_ERROR;
							break;
						}
					}
				} while(retry--);
			}
		} else if (sound_key != SVI_SND_NONE) {
			ret_snd = SVI_ERROR;
		}
	}

	return ret_snd;
}

API int svi_play_vib(int handle, vibration_type vibration_key)
{
	int ret_vib = SVI_SUCCESS;
	int vib_lev = 0;

	if (handle < 0) {
		SVILOG("ERROR!! Please call svi_init() for vibration init ");
		ret_vib = SVI_ERROR;
	} else 	{
		if (vibration_key > SVI_VIB_NONE && vibration_key < SVI_VIB_ENUM_END) {

			if (vibration_key == SVI_VIB_OPERATION_FULLCHARGED || vibration_key == SVI_VIB_OPERATION_LOWBATT) {
				vib_lev = 5;
			} else {
				vib_lev = vib_level;
			}

			SVILOG("key = %d, vibstatus = %d, vib_level = %d", vibration_key, vibstatus, vib_lev);

			if (vibstatus != 0) {

				struct stat buf;
				if(stat(haptic_file[vibration_key], &buf)) { /*check file existence*/
					SVILOG("ERROR!! %s is not presents", haptic_file[vibration_key]);
					if(__svi_restore_default_file(SVI_TYPE_VIB, vibration_key) == SVI_ERROR) {
						SVILOG("ERROR!! __svi_restore_default_file(%d/%d) error", SVI_TYPE_VIB, vibration_key);
						return SVI_ERROR;
					} else {
						SVILOG("%s is restored", haptic_file[vibration_key]);
					}
				}

				int ret = 0;
				ret = device_haptic_play_file(handle, haptic_file[vibration_key], 1, vib_lev);
				if(ret < 0) {
					SVILOG("ERROR!! device_haptic_play_file(%s) returned error(%d).", haptic_file[vibration_key], ret);
					return SVI_ERROR;
				}
			}
		} else {
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

API int svi_set_path(int svi_type, int svi_enum, char* path)
{
	vibration_type vib_enum_type = SVI_VIB_NONE;
	sound_type snd_enum_type = SVI_SND_NONE;
	const char* cur_path = NULL;

	if(svi_type <=SVI_TYPE_NONE || svi_type >= SVI_TYPE_END) {
		SVILOG("ERROR!! invalid svi_type(%d).", svi_type);
		return SVI_ERROR;
	}

	if (path == NULL) {
		SVILOG("ERROR!! invalid path param.");
		return SVI_ERROR;
	} else {
		if(access(path, F_OK) != 0) {
			SVILOG("ERROR!! path does not exist.");
			return SVI_ERROR;
		}
	}

	if (svi_type == SVI_TYPE_SND) {
		snd_enum_type = (sound_type)svi_enum;

		if (snd_enum_type <= SVI_SND_NONE || snd_enum_type >= SVI_SND_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", snd_enum_type);
			return SVI_ERROR;
		}

		cur_path = snd_file[snd_enum_type];
	} else {
		vib_enum_type = (vibration_type)svi_enum;

		if (vib_enum_type <= SVI_VIB_NONE || vib_enum_type >= SVI_VIB_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", vib_enum_type);
			return SVI_ERROR;
		}

		cur_path = haptic_file[vib_enum_type];
	}

	if( (cur_path == NULL) || (strlen(cur_path) == 0) ) {
		SVILOG("ERROR! current path is invalid");
		return SVI_ERROR;
	}

	if(unlink(cur_path) < 0) {
		SVILOG("ERROR!! unlink(%s) error(%d)", cur_path, errno);
		return SVI_ERROR;
	}

	if(symlink(path,cur_path) < 0) {
		SVILOG("ERROR!! symlink(%s) error(%d)", path, errno);
		return SVI_ERROR;
	}

	return SVI_SUCCESS;
}

API int svi_get_path(int svi_type, int svi_enum, char* buf, unsigned int bufLen)
{
	vibration_type vib_enum_type = SVI_VIB_NONE;
	sound_type snd_enum_type = SVI_SND_NONE;
	const char* cur_path = NULL;
	int retry = SVI_RETRY_CNT;

	if (buf == NULL || bufLen <= 0) {
		SVILOG("ERROR!! invalid input parameters.");
		return SVI_ERROR;
	}

	if (!(svi_type == SVI_TYPE_SND || svi_type == SVI_TYPE_VIB)) {
		SVILOG("ERROR!! invalid svi_type(%d).", svi_type);
		return SVI_ERROR;
	}

	if (svi_type == SVI_TYPE_SND) {
		snd_enum_type = (sound_type)svi_enum;

		if (snd_enum_type <= SVI_SND_NONE || snd_enum_type >= SVI_SND_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", snd_enum_type);
			return SVI_ERROR;
		}

		cur_path = snd_file[snd_enum_type];

	} else if (svi_type == SVI_TYPE_VIB) {
		vib_enum_type = (vibration_type) svi_enum;

		if (vib_enum_type <= SVI_VIB_NONE || vib_enum_type >= SVI_VIB_ENUM_END) {
			SVILOG("ERROR! invalid svi_enum(%d)", vib_enum_type);
			return SVI_ERROR;
		}

		cur_path = haptic_file[vib_enum_type];
	}

	do {
		if(readlink(cur_path, buf, bufLen) < 0) {
			if(errno == ENOENT) {
				/* restore svi origin path because of invalid link */
				if(__svi_restore_default_file(svi_type, svi_enum) == SVI_ERROR) {
					SVILOG("ERROR!! __svi_restore_default_file(%d/%d) error", svi_type, svi_enum);
					return SVI_ERROR;
				}
			} else {
				SVILOG("ERROR!! readlink(%s) error(%d)", cur_path, errno);
				return SVI_ERROR;
			}
		}
	} while(retry--);

	return SVI_SUCCESS;
}

