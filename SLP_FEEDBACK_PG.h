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

/**
 *
 * @ingroup	SLP_PG
 * @defgroup    SVI_PG SVI
@{

@par SVI Programming Guide

<h1 class="pg">Brief Information</h1>
- Sound & Vibration UI
- Convenient API
- Header File : svi.h

<h1 class="pg"> Introduction</h1>
<h2 class="pg"> Purpose of this document</h2>
The purpose of this document is to describe how applications can use Sound and Vibration Interface Library APIs.\n
This document gives only programming guidelines to application engineers.

<h2 class="pg"> Scope</h2>
The scope of this document is limited to Samsung platform Sound and Vibration Interface Library API usage.

<h1 class="pg"> Architecture</h1>
<h2 class="pg"> Architecture overview</h2>
Sound and Vibration Interface Library (SVI) is responsible for playing simple sound and vibration.\n
SVI play sound using mm-sound library and play vibration with device framework.

@image html svi.png "SVI diagram" 

<h2 class="pg"> SLP Features</h2>
Sound and Vibration Interface Library has the following features:\n

 - Play Sound
	- It can play simple sound with predefined key.

 - Play Vibration
	- It can play simple vibration with predefined key.

<h2 class="pg">API list and description</h2>
 - int svi_init(int *handle)
 	- Unload sound profile and status, and close haptic device.

 - int svi_fini(int handle)
	- Finalize audio ui library.
	- Unload sound profile and status, and close haptic device.

 - int svi_play_sound(int handle, sound_type sound_key)
 	- Play simple sound.

 - int svi_play_vib(int handle, vibration_type vibration_key)
 	- Play simple vibration.

 - int svi_play(int handle, vibration_type vibration_key, sound_type sound_key)
 	- Play simple sound and vibration.

<h1 class="pg">Programming with Sound & Vibration UI</h1>

<h2 class="pg">Defines</h2>
@code
#define SVI_SUCCESS	0
#define SVI_ERROR	-1
@endcode

<h2 class="pg">Functions</h2>
@code
int svi_init(int *handle)
int svi_fini(int handle)
int svi_play_sound(int handle, sound_type sound_key)
int svi_play_vib(int handle, vibration_type vibration_key)
int svi_play(int handle, vibration_type vibration_key, sound_type sound_key)
@endcode

<h2 class="pg">Example code</h2>
@code
#include <svi.h>

int r = 0;
int handle = 0;

r = svi_init(&handle);
if (r != SVI_SUCCESS){

	printf("Cannot initialize svi.\n");	

} else {

	r = svi_play (handle, SVI_VIB_TOUCH, SVI_SND_TOUCH1);
	if (r != SVI_SUCCESS) {
		printf("Cannot play sound or vibration.\n");
	}

	r = svi_play_sound (handle, SVI_SND_TOUCH1);
	if (r != SVI_SUCCESS) {
		printf("Cannot play sound.\n");
	}

	r = svi_play_vib (handle, SVI_VIB_TOUCH);
	if (r != SVI_SUCCESS) {
		printf("Cannot play vibration.\n");
	}

	r = svi_fini(handle);
	if (r != SVI_SUCCESS) {
		printf("Cannot close svi.\n");	
	}
}
@endcode

<h2 class="pg">Vibration IDs</h2>
@code
	SVI_VIB_TOUCH_TOUCH
	SVI_VIB_TOUCH_SIP
	SVI_VIB_TOUCH_HOLD
	SVI_VIB_TOUCH_MULTI_TAP
	SVI_VIB_TOUCH_HW_TAP
	SVI_VIB_TOUCH_HW_HOLD
	SVI_VIB_TOUCH_KEY00
	SVI_VIB_TOUCH_KEY01
	SVI_VIB_TOUCH_KEY02
	SVI_VIB_TOUCH_KEY03
	SVI_VIB_TOUCH_KEY04
	SVI_VIB_TOUCH_KEY05
	SVI_VIB_TOUCH_KEY06
	SVI_VIB_TOUCH_KEY07
	SVI_VIB_TOUCH_KEY08
	SVI_VIB_TOUCH_KEY09
	SVI_VIB_TOUCH_KEY_STAR
	SVI_VIB_TOUCH_KEY_SHARP
	SVI_VIB_NOTIFICATION_INCOMING_CALL01
	SVI_VIB_NOTIFICATION_INCOMING_CALL02
	SVI_VIB_NOTIFICATION_INCOMING_CALL03
	SVI_VIB_NOTIFICATION_MESSAGE
	SVI_VIB_NOTIFICATION_EMAIL
	SVI_VIB_NOTIFICATION_WAKEUP
	SVI_VIB_NOTIFICATION_SCHEDULE
	SVI_VIB_NOTIFICATION_TIMER
	SVI_VIB_NOTIFICATION_GENERAL
	SVI_VIB_OPERATION_POWER_ON
	SVI_VIB_OPERATION_POWER_OFF
	SVI_VIB_OPERATION_CHARGERCONN
	SVI_VIB_OPERATION_FULLCHARGED
	SVI_VIB_OPERATION_LOWBATT
	SVI_VIB_OPERATION_LOCK
	SVI_VIB_OPERATION_UNLOCK
	SVI_VIB_OPERATION_LOCK_SWIPE
	SVI_VIB_OPERATION_UNLOCK_SWIPE
	SVI_VIB_OPERATION_CALLCONNECT
	SVI_VIB_OPERATION_DISCALLCONNECT
	SVI_VIB_OPERATION_MINUTEMINDER
	SVI_VIB_OPERATION_VIBRATION
	SVI_VIB_OPERATION_NEWCHAT
	SVI_VIB_OPERATION_SENDCHAT
	SVI_VIB_OPERATION_ONOFFSLIDER
	SVI_VIB_OPERATION_SHUTTER
	SVI_VIB_OPERATION_HOURLY_ALERT
@endcode

<h2 class="pg">Sound IDs</h2>
@code
	SVI_SND_TOUCH_TOUCH1
	SVI_SND_TOUCH_TOUCH2
	SVI_SND_TOUCH_TOUCH3
	SVI_SND_TOUCH_SIP
	SVI_SND_TOUCH_HOLD
	SVI_SND_TOUCH_MULTI_TAP
	SVI_SND_TOUCH_HW_TAP
	SVI_SND_TOUCH_HW_HOLD
	SVI_SND_TOUCH_KEY00
	SVI_SND_TOUCH_KEY01
	SVI_SND_TOUCH_KEY02
	SVI_SND_TOUCH_KEY03
	SVI_SND_TOUCH_KEY04
	SVI_SND_TOUCH_KEY05
	SVI_SND_TOUCH_KEY06
	SVI_SND_TOUCH_KEY07
	SVI_SND_TOUCH_KEY08
	SVI_SND_TOUCH_KEY09
	SVI_SND_TOUCH_KEY_STAR
	SVI_SND_TOUCH_KEY_SHARP
	SVI_SND_OPERATION_POWERON
	SVI_SND_OPERATION_POWEROF
	SVI_SND_OPERATION_CHARGERCONN
	SVI_SND_OPERATION_FULLCHARGED
	SVI_SND_OPERATION_LOWBATT
	SVI_SND_OPERATION_LOCK
	SVI_SND_OPERATION_UNLOCK
	SVI_SND_OPERATION_LOCK_SWIPE
	SVI_SND_OPERATION_UNLOCK_SWIPE
	SVI_SND_OPERATION_CALLCONN
	SVI_SND_OPERATION_CALLDISCONN
	SVI_SND_OPERATION_MINUTE_MINDER
	SVI_SND_OPERATION_VIBRATION
	SVI_SND_OPERATION_NEWCHAT
	SVI_SND_OPERATION_SENTCHAT
	SVI_SND_OPERATION_ONOFFSLIDER
	SVI_SND_OPERATION_SCRCAPTURE
	SVI_SND_OPERATION_HOURLY_ALERT
@endcode

*/

/**
@}
*/
