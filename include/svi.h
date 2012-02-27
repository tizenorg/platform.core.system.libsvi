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

#ifndef __SVI_H__
#define __SVI_H__

/**
 * @addtogroup APPLICATION_FRAMEWORK
 * @{
 *
 * @defgroup    SVI SVI 
 * @brief       A library for playing sound and vibration.
 *
 * @section Header To use Them:
 * @code
 * #include <svi.h>
 * @endcode
 *
 * @section Vib_IDs Vibration IDs
 * @code
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
	SVI_VIB_OPERATION_FULLYCHARGED
	SVI_VIB_OPERATION_LOTBATT
	SVI_VIB_OPERATION_LOCK
	SVI_VIB_OPERATION_UNLOCK
	SVI_VIB_OPERATION_CALLCONNECT
	SVI_VIB_OPERATION_DISCALLCONNECT
	SVI_VIB_OPERATION_MINUTEMINDER
	SVI_VIB_OPERATION_VIBRATION
	SVI_VIB_OPERATION_NEWCHAT
	SVI_VIB_OPERATION_SENDCHAT
	SVI_VIB_OPERATION_ONOFFSLIDER
	SVI_VIB_OPERATION_SHUTTER
 * @endcode
 *
 * @section Snd_IDs Sound IDs
 * @code
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
    SVI_SND_OPERATION_POWEROFF
    SVI_SND_OPERATION_CHARGERCONN
    SVI_SND_OPERATION_FULLCHARGED
    SVI_SND_OPERATION_LOWBATT
    SVI_SND_OPERATION_LOCK
    SVI_SND_OPERATION_UNLOCK
    SVI_SND_OPERATION_CALLCONN
    SVI_SND_OPERATION_CALLDISCONN
    SVI_SND_OPERATION_MINUTE_MINDER
    SVI_SND_OPERATION_VIBRATION
    SVI_SND_OPERATION_NEWCHAT
    SVI_SND_OPERATION_SENTCHAT
    SVI_SND_OPERATION_ONOFFSLIDER
    SVI_SND_OPERATION_SCRCAPTURE
 * @endcode
 */

/**
 * @addtogroup SVI
 * @{
 */

#include <svi-ids.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SVI_SUCCESS	0
#define SVI_ERROR	-1

#define SVI_TYPE_SND 0
#define SVI_TYPE_VIB 1

/**
 * \par Description:
 * Initialize audio ui library.\n
 * Load sound profile and status, and open haptic device.\n
 *
 * \par Purpose:
 * This API is used for initializing sound and haptic device.
 *
 * \par Typical use case:
 * If user want to initialize sound or vibration, he(or she) can use this API.
 *
 * \par Method of function operation:
 * SVI uses multimedia sound library and device manager library.
 *
 * \par Important notes:
 * None
 *
 * \param handle	[out]	svi handle.
 *
 * \return Return Type (int) \n
 * - SVI_SUCCESS	- initialize sound status and haptic device successfully. \n
 * - SVI_ERROR	- fail to initialize sound status or haptic device. \n
 *
 * \par Prospective clients:
 * External Apps.
 *
 * \par Related functions:
 * svi_fini() 
 *
 * \par Known issues/bugs:
 * None
 *
 * \pre
 * None
 *
 * \post
 * None
 *
 * \see
 * None
 *
 * \remarks
 * None
 *
 * \par Sample code:
 * \code
 * ...
 * #include <svi.h>
 * ...
 * int r = 0;
 * int handle = 0;
 *
 * r = svi_init(&handle); //Initialize SVI
 * 
 * if ( r != SVI_SUCCESS ) {
 *	printf("Cannot initialize SVI.\n");	
 *	svi_fini(handle); //If initialization is failed, finalize SVI directly
 * } else {
 *
 *	r = svi_play (handle, SVI_VIB_TOUCH_TOUCH, SVI_SND_TOUCH_TOUCH1); //Play sound and vibration 
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot play sound or vibration.\n");
 *	}
 *
 *	r = svi_fini(handle); //Finalize SVI
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot close SVI.\n");	
 *	}
 * }
 *                 
 * ...
 * \endcode
 */
/*================================================================================================*/
int svi_init(int *handle);

/**
 * \par Description:
 * Finalize audio ui library.\n
 * Unload sound profile and status, and close haptic device.\n
 *
 * \par Purpose:
 * This API is used for finalize sound and close haptic device.
 *
 * \par Typical use case:
 * If user want to finalize sound or close haptic device, he(or she) can use this API.
 *
 * \par Method of function operation:
 * SVI uses multimedia sound library and device manager library.
 *
 * \par Important notes:
 * None
 *
 * \param handle	[in]	svi handle.
 *
 * \return Return Type (int) \n
 * - SVI_SUCCESS	- finalize sound status and close haptic device successfully. \n
 * - SVI_ERROR	- fail to finalize sound status or close haptic device. \n
 *
 * \par Prospective clients:
 * External Apps.
 *
 * \par Related functions:
 * svi_init() 
 *
 * \par Known issues/bugs:
 * None
 *
 * \pre
 * None
 *
 * \post
 * None
 *
 * \see
 * None
 *
 * \remarks
 * None
 *
 * \par Sample code:
 * \code
 * ...
 * #include <svi.h>
 * ...
 * int r = 0;
 * int handle = 0;
 *
 * r = svi_init(&handle); //Initialize SVI
 * 
 * if ( r != SVI_SUCCESS ) {
 *	printf("Cannot initialize SVI.\n");	
 *	svi_fini(handle); //If initialization is failed, finalize SVI directly
 * } else {
 *
 *	r = svi_play (handle, SVI_VIB_TOUCH_TOUCH, SVI_SND_TOUCH_TOUCH1); //Play sound and vibration
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot play sound or vibration.\n");
 *	}
 *
 *	r = svi_fini(handle); //Finalize SVI
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot close SVI.\n");	
 *	}
 * }
 *                 
 * ...
 * \endcode
 */
/*================================================================================================*/
int svi_fini(int handle);


/**
 * \par Description:
 * Play simple sound.\n
 *
 * \par Purpose:
 * This API is used for playing predefined simple sound.
 *
 * \par Typical use case:
 * If user want to play sound for predefined simple pattern, he(or she) can use this API.
 *
 * \par Method of function operation:
 * SVI uses multimedia sound library.
 *
 * \par Important notes:
 * None
 *
 * \param handle	[in]	svi handle.
 * \param sound_key	[in]	predefined sound key.
 *
 * \return Return Type (int) \n
 * - SVI_SUCCESS	- play sound successfully. \n
 * - SVI_ERROR	- fail to play sound . \n
 *
 * \par Prospective clients:
 * External Apps.
 *
 * \par Related functions:
 * None
 *
 * \par Known issues/bugs:
 * None
 *
 * \pre
 * None
 *
 * \post
 * None
 *
 * \see
 * None
 *
 * \remarks
 * None
 *
 * \par Sample code:
 * \code
 * ...
 * #include <svi.h>
 * ...
 * int r = 0;
 * int handle = 0;
 * 
 * r = svi_init(&handle); //Initialize SVI
 * 
 * if ( r != SVI_SUCCESS ) {
 *	printf("Cannot initialize SVI.\n");	
 *	svi_fini(handle); //If initialization is failed, finalize SVI directly
 * } else {
 *
 *	r = svi_play_sound (handle, SVI_SND_TOUCH_TOUCH1); //Play sound 
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot play sound or vibration.\n");
 *	}
 *
 *	r = svi_fini(handle); //Finalize SVI
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot close SVI.\n");	
 *	}
 * }
 *                 
 * ...
 * \endcode
 */
/*================================================================================================*/
int svi_play_sound(int handle, sound_type sound_key);

/**
 * \par Description:
 * Play simple vibration.\n
 *
 * \par Purpose:
 * This API is used for playing predefined simple vibration.
 *
 * \par Typical use case:
 * If user want to play vibration for predefined simple pattern, he(or she) can use this API.
 *
 * \par Method of function operation:
 * SVI uses device manager library.
 *
 * \par Important notes:
 * None
 *
 * \param handle	[in]	svi handle.
 * \param vibration_key	[in]	predefined vibration key.
 *
 * \return Return Type (int) \n
 * - SVI_SUCCESS	- play sound and vibration successfully. \n
 * - SVI_ERROR	- fail to play sound or vibration. \n
 *
 * \par Prospective clients:
 * External Apps.
 *
 * \par Related functions:
 * None
 *
 * \par Known issues/bugs:
 * None
 *
 * \pre
 * None
 *
 * \post
 * None
 *
 * \see
 * None
 *
 * \remarks
 * None
 *
 * \par Sample code:
 * \code
 * ...
 * #include <svi.h>
 * ...
 * int r = 0;
 * int handle = 0;
 * 
 * r = svi_init(&handle); //Initialize SVI
 * 
 * if ( r != SVI_SUCCESS ) {
 *	printf("Cannot initialize SVI.\n");	
 *	svi_fini(handle); //If initialization is failed, finalize SVI directly
 * } else {
 *
 *	r = svi_play_vib (handle, SVI_VIB_TOUCH_TOUCH); //Play vibration
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot play sound or vibration.\n");
 *	}
 *
 *	r = svi_fini(handle); //Finalize SVI
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot close SVI.\n");	
 *	}
 * }
 *                 
 * ...
 * \endcode
 */
/*================================================================================================*/
int svi_play_vib(int handle, vibration_type vibration_key);

/**
 * \par Description:
 * Play simple sound and vibration.\n
 *
 * \par Purpose:
 * This API is used for playing predefined simple sound and vibration.
 *
 * \par Typical use case:
 * If user want to play sound or vibration for predefined simple pattern, he(or she) can use this API.
 *
 * \par Method of function operation:
 * SVI uses multimedia sound library and device manager library.
 *
 * \par Important notes:
 * None
 *
 * \param handle	[in]	svi handle.
 * \param vibration_key	[in]	predefined vibration key.
 * \param sound_key	[in]	predefined sound key.
 *
 * \return Return Type (int) \n
 * - SVI_SUCCESS	- play sound and vibration successfully. \n
 * - SVI_ERROR	- fail to play sound or vibration. \n
 *
 * \par Prospective clients:
 * External Apps.
 *
 * \par Related functions:
 * None
 *
 * \par Known issues/bugs:
 * None
 *
 * \pre
 * None
 *
 * \post
 * None
 *
 * \see
 * None
 *
 * \remarks
 * None
 *
 * \par Sample code:
 * \code
 * ...
 * #include <svi.h>
 * ...
 * int r = 0;
 * int handle = 0;
 * 
 * r = svi_init(&handle); //Initialize SVI
 * 
 * if ( r != SVI_SUCCESS ) {
 *	printf("Cannot initialize SVI.\n");	
 *	svi_fini(handle); //If initialization is failed, finalize SVI directly.
 * } else {
 *
 *	r = svi_play (handle, SVI_VIB_TOUCH_TOUCH, SVI_SND_TOUCH_TOUCH1); //Play sound and vibration
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot play sound or vibration.\n");
 *	}
 *
 *	r = svi_fini(handle); //Finalize SVI
 *	if (r != SVI_SUCCESS) {
 *		printf("Cannot close SVI.\n");	
 *	}
 * }
 *                 
 * ...
 * \endcode
 */
/*================================================================================================*/
int svi_play(int handle, vibration_type vibration_key, sound_type sound_key);

/**
 * \par Description:
 * Get filepath from predefined sound & vibration.\n
 *
 * \par Purpose:
 * This API is used for getting filepath of predefined sound or vibration.
 *
 * \par Typical use case:
 * If user want to get filepath of sound or vibration for predefined simple pattern, he(or she) can use this API.
 *
 * \par Method of function operation:
 * SVI uses multimedia sound library and device manager library.
 *
 * \par Important notes:
 * None
 *
 * \param svi_type 	[in]	svi_type(sound, vibration).
 * \param svi_enum	[in]	predefined enum_type.
 * \param buf			[out] allocated buffer.
 * \param bufLen		[out] allocated buffer size.
 *
 * \return Return Type (int) \n
 * - SVI_SUCCESS	- \n
 * - SVI_ERROR	- \n
 *
 * \par Prospective clients:
 * External Apps.
 *
 * \par Related functions:
 * None
 *
 * \par Known issues/bugs:
 * None
 *
 * \pre
 * None
 *
 * \post
 * None
 *
 * \see
 * None
 *
 * \remarks
 * None
 *
 * \par Sample code:
 * \code
 * ...
 * #include <svi.h>
 * ...
 * int r = 0;
 * char buf[256] = {0,};
 * 
 * r = svi_get_path(SVI_SND_TYPE, SVI_SND_TOUCH_TOUCH1, buf, sizeof(buf));
 * 
 * if ( r != SVI_SUCCESS ) {
 *	printf("Fail.\n");	
 * }              
 * ...
 * \endcode
 */
/*================================================================================================*/

int svi_get_path(int svi_type, int svi_enum, char* buf, unsigned int bufLen);

#ifdef __cplusplus
}
#endif

/**
 * @} @}
 */

#endif //__SVI_H__
