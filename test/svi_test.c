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

#include <stdio.h>
#include <string.h>
#include "svi.h"

char* show_sound_type[] = {
    "SVI_SND_TOUCH_TOUCH1",
    "SVI_SND_TOUCH_TOUCH2",
    "SVI_SND_TOUCH_TOUCH3",
    "SVI_SND_TOUCH_SIP",
    "SVI_SND_TOUCH_SIP_BACKSPACE",
    "SVI_SND_TOUCH_HOLD",
    "SVI_SND_TOUCH_MULTI_TAP",
    "SVI_SND_TOUCH_HW_TAP",
    "SVI_SND_TOUCH_HW_HOLD",
    "SVI_SND_TOUCH_KEY00",
    "SVI_SND_TOUCH_KEY01",
    "SVI_SND_TOUCH_KEY02",
    "SVI_SND_TOUCH_KEY03",
    "SVI_SND_TOUCH_KEY04",
    "SVI_SND_TOUCH_KEY05",
    "SVI_SND_TOUCH_KEY06",
    "SVI_SND_TOUCH_KEY07",
    "SVI_SND_TOUCH_KEY08",
    "SVI_SND_TOUCH_KEY09",
    "SVI_SND_TOUCH_KEY_STAR",
    "SVI_SND_TOUCH_KEY_SHARP",
    "SVI_SND_OPERATION_POWERON",
    "SVI_SND_OPERATION_POWEROFF",
    "SVI_SND_OPERATION_CHARGERCONN",
    "SVI_SND_OPERATION_FULLCHARGED",
    "SVI_SND_OPERATION_LOWBATT",
    "SVI_SND_OPERATION_LOCK",
    "SVI_SND_OPERATION_UNLOCK",
    "SVI_SND_OPERATION_CALLCONN",
    "SVI_SND_OPERATION_CALLDISCONN",
    "SVI_SND_OPERATION_MINUTE_MINDER",
    "SVI_SND_OPERATION_VIBRATION",
    "SVI_SND_OPERATION_NEWCHAT",
    "SVI_SND_OPERATION_SENTCHAT",
    "SVI_SND_OPERATION_ONOFFSLIDER",
    "SVI_SND_OPERATION_SCRCAPTURE"
};

char* show_vibration_type[] = {
    "SVI_VIB_TOUCH_TOUCH",
    "SVI_VIB_TOUCH_SIP",
    "SVI_VIB_TOUCH_SIP_BACKSPACE",
    "SVI_VIB_TOUCH_HOLD",
    "SVI_VIB_TOUCH_MULTI_TAP",
    "SVI_VIB_TOUCH_HW_TAP",
    "SVI_VIB_TOUCH_HW_HOLD",
    "SVI_VIB_TOUCH_KEY00",
    "SVI_VIB_TOUCH_KEY01",
    "SVI_VIB_TOUCH_KEY02",
    "SVI_VIB_TOUCH_KEY03",
    "SVI_VIB_TOUCH_KEY04",
    "SVI_VIB_TOUCH_KEY05",
    "SVI_VIB_TOUCH_KEY06",
    "SVI_VIB_TOUCH_KEY07",
    "SVI_VIB_TOUCH_KEY08",
    "SVI_VIB_TOUCH_KEY09",
    "SVI_VIB_TOUCH_KEY_STAR",
    "SVI_VIB_TOUCH_KEY_SHARP",
    "SVI_VIB_NOTIFICATION_INCOMING_CALL01",
    "SVI_VIB_NOTIFICATION_INCOMING_CALL02",
    "SVI_VIB_NOTIFICATION_INCOMING_CALL03",
    "SVI_VIB_NOTIFICATION_INCOMING_CALL04",
    "SVI_VIB_NOTIFICATION_INCOMING_CALL05",
    "SVI_VIB_NOTIFICATION_INCOMING_CALL06",
    "SVI_VIB_NOTIFICATION_MESSAGE",
    "SVI_VIB_NOTIFICATION_EMAIL",
    "SVI_VIB_NOTIFICATION_WAKEUP",
    "SVI_VIB_NOTIFICATION_SCHEDULE",
    "SVI_VIB_NOTIFICATION_TIMER",
    "SVI_VIB_NOTIFICATION_GENERAL",
    "SVI_VIB_OPERATION_POWER_ON",
    "SVI_VIB_OPERATION_POWER_OFF",
    "SVI_VIB_OPERATION_CHARGERCONN",
    "SVI_VIB_OPERATION_FULLCHARGED",
    "SVI_VIB_OPERATION_LOWBATT",
    "SVI_VIB_OPERATION_LOCK",
    "SVI_VIB_OPERATION_UNLOCK",
    "SVI_VIB_OPERATION_CALLCONNECT",
    "SVI_VIB_OPERATION_DISCALLCONNECT",
    "SVI_VIB_OPERATION_MINUTEMINDER",
    "SVI_VIB_OPERATION_VIBRATION",
    "SVI_VIB_OPERATION_NEWCHAT",
    "SVI_VIB_OPERATION_SENDCHAT",
    "SVI_VIB_OPERATION_ONOFFSLIDER",
    "SVI_VIB_OPERATION_SHUTTER"
};

int main(int argc, char* argv[]) {
	int r = 0;
	int i = 0;
	int handle = 0;
	int input_type = 0, input = 0;
	char buf[512] = {0,};
	
	r = svi_init(&handle);

	printf("Select number of vibration(1), sound(2), svi_get_path(3) for testing\n");
	scanf("%d", &input_type);

	if (input_type == 1) {
		do {
			printf("SVI vibration ENUM list :\n");
			for(i = 0; i <= SVI_VIB_OPERATION_SHUTTER; i++) {
				printf("\t %s(%d)\n", show_vibration_type[i], i);
			}

			printf("input number: ");
			scanf("%d",&input);

			if( input < SVI_VIB_TOUCH_TOUCH || input > SVI_VIB_OPERATION_SHUTTER)
				break;
			printf("%s\n", show_vibration_type[input]);
			r = svi_play_vib(handle, input);
			
		} while(1); 
		
	}
	else if (input_type == 2) {
			

		do {
			printf("SVI sound enum list :\n");
			for(i = 0; i <= SVI_SND_OPERATION_SCRCAPTURE; i++) {
				printf("\t %s(%d)\n", show_sound_type[i], i);
			}

			printf("input number: ");
			scanf("%d",&input);

			if( input < SVI_SND_TOUCH_TOUCH1 || input > SVI_SND_OPERATION_SCRCAPTURE)
				break;
			r = svi_play_sound(handle, input);
			
		} while(1);
	} else if (input_type == 3) {
		do {
			int input_type = -1;
			int input_enum = -1;

			memset(buf, 0x0, sizeof(buf));
			printf("input <SVI_TYPE_SND/SVI_TYPE_VIB> <enum> :\n");
			scanf("%d %d",&input_type, &input_enum);

			if( !(input_type == SVI_TYPE_SND || input_type == SVI_TYPE_VIB)) {
				printf("wrong input_type!!!\n");
				break;
			}

			if(input_type == SVI_TYPE_SND) {
				if( input_enum < SVI_SND_TOUCH_TOUCH1 || input_enum > SVI_SND_OPERATION_SCRCAPTURE) {
					printf("wrong input_enum!!!\n");
					break;
				}
			} else if(input_type == SVI_TYPE_VIB) {
				if( input_enum < SVI_VIB_TOUCH_TOUCH || input_enum > SVI_VIB_OPERATION_SHUTTER) {
					printf("wrong input_enum!!!\n");
					break;
				}
			}
			r = svi_get_path(input_type, input_enum, buf, sizeof(buf));
			printf("buf is %s\n", buf);
		} while(1);
	} else {
		printf("wrong number!!!\n");
		r = svi_fini(handle);
		return 0;
	}

	printf("end!!\n");
	r = svi_fini(handle);
	return 0;
}

