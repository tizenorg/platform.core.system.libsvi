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


#ifndef __FEEDBACK_IDS_H__
#define __FEEDBACK_IDS_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @addtogroup CAPI_SYSTEM_FEEDBACK_MODULE
 * @{
 */

/**
 * @brief Enumerations of the type for feedback interface
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
typedef enum {
	FEEDBACK_TYPE_NONE,      			/**< Feedback type none */
	FEEDBACK_TYPE_SOUND,    			/**< Feedback type for sound */
	FEEDBACK_TYPE_VIBRATION,			/**< Feedback type for vibration */
	FEEDBACK_TYPE_END,
} feedback_type_e;

/**
 * @brief Enumerations of the system pre-defined patterns for feedback interface
 * @details
 * Each feedback pattern can have separate media files of each types.
 * But Depending on vendor design, pattern may not have any type of file.
 *
 * @since_tizen @if MOBILE 2.4 @elseif WEARABLE 3.0 @endif
 */
typedef enum {
	FEEDBACK_PATTERN_NONE = -1,

	FEEDBACK_PATTERN_TAP = 0,      					/**< Feedback pattern when general touch */
	FEEDBACK_PATTERN_SIP,          					/**< Feedback pattern when touch text key */
	FEEDBACK_PATTERN_KEY0 = 6,     					/**< Feedback pattern when touch numeric 0 key */
	FEEDBACK_PATTERN_KEY1,         					/**< Feedback pattern when touch numeric 1 key */
	FEEDBACK_PATTERN_KEY2,         					/**< Feedback pattern when touch numeric 2 key */
	FEEDBACK_PATTERN_KEY3,         					/**< Feedback pattern when touch numeric 3 key */
	FEEDBACK_PATTERN_KEY4,         					/**< Feedback pattern when touch numeric 4 key */
	FEEDBACK_PATTERN_KEY5,         					/**< Feedback pattern when touch numeric 5 key */
	FEEDBACK_PATTERN_KEY6,         					/**< Feedback pattern when touch numeric 6 key */
	FEEDBACK_PATTERN_KEY7,         					/**< Feedback pattern when touch numeric 7 key */
	FEEDBACK_PATTERN_KEY8,         					/**< Feedback pattern when touch numeric 8 key */
	FEEDBACK_PATTERN_KEY9,         					/**< Feedback pattern when touch numeric 9 key */
	FEEDBACK_PATTERN_KEY_STAR,     					/**< Feedback pattern when touch star key */
	FEEDBACK_PATTERN_KEY_SHARP,    					/**< Feedback pattern when touch sharp key */
	FEEDBACK_PATTERN_KEY_BACK,     					/**< Feedback pattern when touch backspace key */
	FEEDBACK_PATTERN_HOLD,         					/**< Feedback pattern when touch hold */
	FEEDBACK_PATTERN_HW_TAP = 21,  					/**< Feedback pattern when press hardware key */
	FEEDBACK_PATTERN_HW_HOLD,      					/**< Feedback pattern when holding press hardware key */

	FEEDBACK_PATTERN_MESSAGE,         				/**< Feedback pattern when incoming a message */
	FEEDBACK_PATTERN_EMAIL = 25,           				/**< Feedback pattern when incoming an email */
	FEEDBACK_PATTERN_WAKEUP = 27,          				/**< Feedback pattern when alert wake up call */
	FEEDBACK_PATTERN_SCHEDULE = 29,        				/**< Feedback pattern when alert schedule alarm */
	FEEDBACK_PATTERN_TIMER = 31,           				/**< Feedback pattern when alert timer */
	FEEDBACK_PATTERN_GENERAL = 33,         				/**< Feedback pattern when alert general event */

	FEEDBACK_PATTERN_POWERON = 36,            			/**< Feedback pattern when power on */
	FEEDBACK_PATTERN_POWEROFF,         				/**< Feedback pattern when power off */
	FEEDBACK_PATTERN_CHARGERCONN,           			/**< Feedback pattern when connecting charger */
	FEEDBACK_PATTERN_CHARGING_ERROR = 40,     			/**< Feedback pattern when occuring charging error */
	FEEDBACK_PATTERN_FULLCHARGED = 42,        			/**< Feedback pattern when full charged */
	FEEDBACK_PATTERN_LOWBATT = 44,            			/**< Feedback pattern when low battery */
	FEEDBACK_PATTERN_LOCK = 46,               			/**< Feedback pattern when lock */
	FEEDBACK_PATTERN_UNLOCK,             				/**< Feedback pattern when unlock */
	FEEDBACK_PATTERN_VIBRATION_ON = 55,       			/**< Feedback pattern when turn on vibration mode */
	FEEDBACK_PATTERN_SILENT_OFF,         				/**< Feedback pattern when turn off silent mode */
	FEEDBACK_PATTERN_BT_CONNECTED,       				/**< Feedback pattern when connecting with bluetooth */
	FEEDBACK_PATTERN_BT_DISCONNECTED,    				/**< Feedback pattern when disconnecting with bluetooth */
	FEEDBACK_PATTERN_LIST_REORDER = 62,       			/**< Feedback pattern when list reorder */
	FEEDBACK_PATTERN_LIST_SLIDER,        				/**< Feedback pattern when list slider sweep */
	FEEDBACK_PATTERN_VOLUME_KEY,         				/**< Feedback pattern when pressed volume key */

	FEEDBACK_PATTERN_END,

} feedback_pattern_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_H__
