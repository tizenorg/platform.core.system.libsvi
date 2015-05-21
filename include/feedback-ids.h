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
 * @since_tizen 2.4
 */
typedef enum
{
	FEEDBACK_TYPE_NONE,
	FEEDBACK_TYPE_SOUND,
	FEEDBACK_TYPE_VIBRATION,
	FEEDBACK_TYPE_END,
	/* TODO Will be removed */
	FEEDBACK_TYPE_LED = FEEDBACK_TYPE_NONE,
} feedback_type_e;

/**
 * @brief Enumerations of the system pre-defined patterns for feedback interface
 * @details
 * Each feedback pattern can have separate media files of each types.
 * But Depending on vendor design, pattern may not have any type of file.
 *
 * @since_tizen 2.4
 */
typedef enum
{
	FEEDBACK_PATTERN_NONE = -1,

	FEEDBACK_PATTERN_TAP = 0,      			/**< Feedback pattern when general touch */
	FEEDBACK_PATTERN_SIP,          			/**< Feedback pattern when touch text key */
	FEEDBACK_PATTERN_KEY0,         			/**< Feedback pattern when touch numeric 0 key */
	FEEDBACK_PATTERN_KEY1,         			/**< Feedback pattern when touch numeric 1 key */
	FEEDBACK_PATTERN_KEY2,         			/**< Feedback pattern when touch numeric 2 key */
	FEEDBACK_PATTERN_KEY3,         			/**< Feedback pattern when touch numeric 3 key */
	FEEDBACK_PATTERN_KEY4,         			/**< Feedback pattern when touch numeric 4 key */
	FEEDBACK_PATTERN_KEY5,         			/**< Feedback pattern when touch numeric 5 key */
	FEEDBACK_PATTERN_KEY6,         			/**< Feedback pattern when touch numeric 6 key */
	FEEDBACK_PATTERN_KEY7,         			/**< Feedback pattern when touch numeric 7 key */
	FEEDBACK_PATTERN_KEY8,         			/**< Feedback pattern when touch numeric 8 key */
	FEEDBACK_PATTERN_KEY9,         			/**< Feedback pattern when touch numeric 9 key */
	FEEDBACK_PATTERN_KEY_STAR,     			/**< Feedback pattern when touch star key */
	FEEDBACK_PATTERN_KEY_SHARP,    			/**< Feedback pattern when touch sharp key */
	FEEDBACK_PATTERN_KEY_BACK,     			/**< Feedback pattern when touch backspace key */
	FEEDBACK_PATTERN_HOLD,         			/**< Feedback pattern when touch hold */
	FEEDBACK_PATTERN_HW_TAP,       			/**< Feedback pattern when press hardware key */
	FEEDBACK_PATTERN_HW_HOLD,      			/**< Feedback pattern when holding press hardware key */

	FEEDBACK_PATTERN_MESSAGE,         		/**< Feedback pattern when incoming a message */
	FEEDBACK_PATTERN_EMAIL,           		/**< Feedback pattern when incoming an email */
	FEEDBACK_PATTERN_WAKEUP,          		/**< Feedback pattern when alert wake up call */
	FEEDBACK_PATTERN_SCHEDULE,        		/**< Feedback pattern when alert schedule alarm */
	FEEDBACK_PATTERN_TIMER,           		/**< Feedback pattern when alert timer */
	FEEDBACK_PATTERN_GENERAL,         		/**< Feedback pattern when alert general event */

	FEEDBACK_PATTERN_POWERON,            	/**< Feedback pattern when power on */
	FEEDBACK_PATTERN_POWEROFF,           	/**< Feedback pattern when power off */
	FEEDBACK_PATTERN_CHARGERCONN,        	/**< Feedback pattern when connecting charger */
	FEEDBACK_PATTERN_CHARGING_ERROR,     	/**< Feedback pattern when occuring charging error */
	FEEDBACK_PATTERN_FULLCHARGED,        	/**< Feedback pattern when full charged */
	FEEDBACK_PATTERN_LOWBATT,            	/**< Feedback pattern when low battery */
	FEEDBACK_PATTERN_LOCK,               	/**< Feedback pattern when lock */
	FEEDBACK_PATTERN_UNLOCK,             	/**< Feedback pattern when unlock */
	FEEDBACK_PATTERN_VIBRATION_ON,       	/**< Feedback pattern when turn on vibration mode */
	FEEDBACK_PATTERN_SILENT_OFF,         	/**< Feedback pattern when turn off silent mode */
	FEEDBACK_PATTERN_BT_CONNECTED,       	/**< Feedback pattern when connecting with bluetooth */
	FEEDBACK_PATTERN_BT_DISCONNECTED,    	/**< Feedback pattern when disconnecting with bluetooth */
	FEEDBACK_PATTERN_LIST_REORDER,       	/**< Feedback pattern when list reorder */
	FEEDBACK_PATTERN_LIST_SLIDER,        	/**< Feedback pattern when list slider sweep */
	FEEDBACK_PATTERN_VOLUME_KEY,         	/**< Feedback pattern when pressed volume key */

	FEEDBACK_PATTERN_END,

	/* TODO Will be removed */
	FEEDBACK_PATTERN_TOUCH_TAP = FEEDBACK_PATTERN_TAP,
	FEEDBACK_PATTERN_TOUCH_MULTI_TAP = FEEDBACK_PATTERN_TAP,
	FEEDBACK_PATTERN_TOUCH_KEY = FEEDBACK_PATTERN_KEY0,
	FEEDBACK_PATTERN_TOUCH_HOLD = FEEDBACK_PATTERN_HOLD,
	FEEDBACK_PATTERN_REACTIVE_ALERT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_VIBRATION = FEEDBACK_PATTERN_VIBRATION_ON,
	FEEDBACK_PATTERN_SOUND_ON = FEEDBACK_PATTERN_SILENT_OFF,
	FEEDBACK_PATTERN_SHUTTER = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SLIDER_SWEEP = FEEDBACK_PATTERN_LIST_SLIDER,
	FEEDBACK_PATTERN_SAFETY_ASSISTANCE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_BT_PARING = FEEDBACK_PATTERN_BT_CONNECTED,
	FEEDBACK_PATTERN_CONNECTED = FEEDBACK_PATTERN_BT_CONNECTED,
	FEEDBACK_PATTERN_DISCONNECTED = FEEDBACK_PATTERN_BT_DISCONNECTED,
	FEEDBACK_PATTERN_SUCCESS = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_FAILURE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SIP_BACKSPACE = FEEDBACK_PATTERN_SIP,
	FEEDBACK_PATTERN_SIP_FUNCTION = FEEDBACK_PATTERN_SIP,
	FEEDBACK_PATTERN_SIP_FJKEY = FEEDBACK_PATTERN_SIP,
	FEEDBACK_PATTERN_MAX_CHARACTER = FEEDBACK_PATTERN_SIP,
	FEEDBACK_PATTERN_MULTI_TAP = FEEDBACK_PATTERN_TAP,
	FEEDBACK_PATTERN_MESSAGE_ON_CALL = FEEDBACK_PATTERN_MESSAGE,
	FEEDBACK_PATTERN_EMAIL_ON_CALL = FEEDBACK_PATTERN_EMAIL,
	FEEDBACK_PATTERN_WAKEUP_ON_CALL = FEEDBACK_PATTERN_WAKEUP,
	FEEDBACK_PATTERN_SCHEDULE_ON_CALL = FEEDBACK_PATTERN_SCHEDULE,
	FEEDBACK_PATTERN_TIMER_ON_CALL = FEEDBACK_PATTERN_TIMER,
	FEEDBACK_PATTERN_GENERAL_ON_CALL = FEEDBACK_PATTERN_GENERAL,
	FEEDBACK_PATTERN_SMART_ALERT = FEEDBACK_PATTERN_GENERAL,
	FEEDBACK_PATTERN_CHARGERCONN_ON_CALL = FEEDBACK_PATTERN_CHARGERCONN,
	FEEDBACK_PATTERN_CHARGING_ERROR_ON_CALL = FEEDBACK_PATTERN_CHARGING_ERROR,
	FEEDBACK_PATTERN_FULLCHARGED_ON_CALL = FEEDBACK_PATTERN_FULLCHARGED,
	FEEDBACK_PATTERN_LOWBATT_ON_CALL = FEEDBACK_PATTERN_LOWBATT,
	FEEDBACK_PATTERN_LOCK_SWIPE = FEEDBACK_PATTERN_LOCK,
	FEEDBACK_PATTERN_UNLOCK_SWIPE = FEEDBACK_PATTERN_UNLOCK,
	FEEDBACK_PATTERN_GEOMETRIC_LOCK = FEEDBACK_PATTERN_LOCK,
	FEEDBACK_PATTERN_CALLCONNECT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_DISCALLCONNECT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_OUTGOING_CALL = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_MINUTEMINDER = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_BT_PAIRING = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_BT_WAITING = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SCREEN_CAPTURE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_MMS = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_HOURLY_ALERT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SAFETY_ALERT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_ACCIDENT_DETECT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SEND_SOS_MESSAGE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_END_SOS_MESSAGE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_EMERGENCY_BUZZER = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SAFETY_LOW_POWER = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_CMAS = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SPEED_UP = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SLOW_DOWN = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_KEEP_THIS_PACE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_GOAL_ACHIEVED = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_EXERCISE_COUNT = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_START_CUE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_HEALTH_PACE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_INACTIVE_TIME = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_MEASURING_SUCCESS = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_MEASURING_FAILURE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_UV_PROCESSING = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SHEALTH_START = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SHEALTH_PAUSE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_SHEALTH_STOP = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_3RD_APPLICATION = FEEDBACK_PATTERN_NONE,
	/* END : Will be removed */

} feedback_pattern_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_H__
