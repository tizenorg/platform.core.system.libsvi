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
 * @file feedback-ids.h
 * @brief This file contains the feedback API
 */

/**
 * @addtogroup CAPI_FEEDBACK_MODULE
 * @{
 */

/**
 * @brief Enumerations of the type for feedback interface
 * @details
 *
 */
typedef enum
{
	FEEDBACK_TYPE_NONE,
	FEEDBACK_TYPE_SOUND,
	FEEDBACK_TYPE_VIBRATION,
	FEEDBACK_TYPE_LED,
	FEEDBACK_TYPE_END
} feedback_type_e;

/**
 * @brief Enumerations of the system pre-defined patterns for feedback interface
 * @details
 * Each feedback pattern can have separate media files of each types.
 * But Depending on vendor design, pattern may not have any type of file.
 *
 */
typedef enum
{
	FEEDBACK_PATTERN_NONE = -1,

	FEEDBACK_PATTERN_TAP = 0,      			/**< feedback pattern when general touch */
	FEEDBACK_PATTERN_SIP,          			/**< feedback pattern when touch text key */
	FEEDBACK_PATTERN_SIP_BACKSPACE,			/**< feedback pattern when touch backspace key */
	FEEDBACK_PATTERN_SIP_FUNCTION, 			/**< feedback pattern when touch function key */
	FEEDBACK_PATTERN_SIP_FJKEY,    			/**< feedback pattern when touch F,J key */
	FEEDBACK_PATTERN_MAX_CHARACTER,			/**< feedback pattern when max character */
	FEEDBACK_PATTERN_KEY0,         			/**< feedback pattern when touch numeric 0 key */
	FEEDBACK_PATTERN_KEY1,         			/**< feedback pattern when touch numeric 1 key */
	FEEDBACK_PATTERN_KEY2,         			/**< feedback pattern when touch numeric 2 key */
	FEEDBACK_PATTERN_KEY3,         			/**< feedback pattern when touch numeric 3 key */
	FEEDBACK_PATTERN_KEY4,         			/**< feedback pattern when touch numeric 4 key */
	FEEDBACK_PATTERN_KEY5,         			/**< feedback pattern when touch numeric 5 key */
	FEEDBACK_PATTERN_KEY6,         			/**< feedback pattern when touch numeric 6 key */
	FEEDBACK_PATTERN_KEY7,         			/**< feedback pattern when touch numeric 7 key */
	FEEDBACK_PATTERN_KEY8,         			/**< feedback pattern when touch numeric 8 key */
	FEEDBACK_PATTERN_KEY9,         			/**< feedback pattern when touch numeric 9 key */
	FEEDBACK_PATTERN_KEY_STAR,     			/**< feedback pattern when touch star key */
	FEEDBACK_PATTERN_KEY_SHARP,    			/**< feedback pattern when touch sharp key */
	FEEDBACK_PATTERN_KEY_BACK,     			/**< feedback pattern when touch backspace key */
	FEEDBACK_PATTERN_HOLD,         			/**< feedback pattern when touch hold */
	FEEDBACK_PATTERN_MULTI_TAP,    			/**< feedback pattern when multi touch */
	FEEDBACK_PATTERN_HW_TAP,       			/**< feedback pattern when press hardware key */
	FEEDBACK_PATTERN_HW_HOLD,      			/**< feedback pattern when holding press hardware key */

	FEEDBACK_PATTERN_MESSAGE,         		/**< feedback pattern when incoming a message */
	FEEDBACK_PATTERN_MESSAGE_ON_CALL, 		/**< feedback pattern when incoming a message on call */
	FEEDBACK_PATTERN_EMAIL,           		/**< feedback pattern when incoming an email */
	FEEDBACK_PATTERN_EMAIL_ON_CALL,   		/**< feedback pattern when incoming an email on call */
	FEEDBACK_PATTERN_WAKEUP,          		/**< feedback pattern when alert wake up call */
	FEEDBACK_PATTERN_WAKEUP_ON_CALL,  		/**< feedback pattern when alert wake up call on call */
	FEEDBACK_PATTERN_SCHEDULE,        		/**< feedback pattern when alert schedule alarm */
	FEEDBACK_PATTERN_SCHEDULE_ON_CALL,		/**< feedback pattern when alert schedule alarm on call */
	FEEDBACK_PATTERN_TIMER,           		/**< feedback pattern when alert timer */
	FEEDBACK_PATTERN_TIMER_ON_CALL,   		/**< feedback pattern when alert timer on call */
	FEEDBACK_PATTERN_GENERAL,         		/**< feedback pattern when alert general event */
	FEEDBACK_PATTERN_GENERAL_ON_CALL, 		/**< feedback pattern when alert general event on call */
	FEEDBACK_PATTERN_SMART_ALERT,     		/**< feedback pattern when alert noti on motion */

	FEEDBACK_PATTERN_POWERON,            	/**< feedback pattern when power on */
	FEEDBACK_PATTERN_POWEROFF,           	/**< feedback pattern when power off */
	FEEDBACK_PATTERN_CHARGERCONN,        	/**< feedback pattern when connecting charger */
	FEEDBACK_PATTERN_CHARGERCONN_ON_CALL,	/**< feedback pattern when connecting charger on call */
	FEEDBACK_PATTERN_CHARGING_ERROR,     	/**< feedback pattern when occuring charging error */
	FEEDBACK_PATTERN_CHARGING_ERROR_ON_CALL,/**< feedback pattern when occuring charging error on call */
	FEEDBACK_PATTERN_FULLCHARGED,        	/**< feedback pattern when full charged */
	FEEDBACK_PATTERN_FULLCHARGED_ON_CALL,	/**< feedback pattern when full charged on call */
	FEEDBACK_PATTERN_LOWBATT,            	/**< feedback pattern when low battery */
	FEEDBACK_PATTERN_LOWBATT_ON_CALL,    	/**< feedback pattern when low battery on call */
	FEEDBACK_PATTERN_LOCK,               	/**< feedback pattern when lock */
	FEEDBACK_PATTERN_UNLOCK,             	/**< feedback pattern when unlock */
	FEEDBACK_PATTERN_LOCK_SWIPE,         	/**< feedback pattern when lock swipe */
	FEEDBACK_PATTERN_UNLOCK_SWIPE,       	/**< feedback pattern when unlock swipe*/
	FEEDBACK_PATTERN_GEOMETRIC_LOCK,     	/**< feedback pattern when lock by geometric effect */
	FEEDBACK_PATTERN_CALLCONNECT,        	/**< feedback pattern when connecting call */
	FEEDBACK_PATTERN_DISCALLCONNECT,     	/**< feedback pattern when disconnecting call */
	FEEDBACK_PATTERN_OUTGOING_CALL,      	/**< feedback pattern when connected outgoing call */
	FEEDBACK_PATTERN_MINUTEMINDER,       	/**< feedback pattern when minute minder */
	FEEDBACK_PATTERN_VIBRATION_ON,       	/**< feedback pattern when turn on vibration mode */
	FEEDBACK_PATTERN_SILENT_OFF,         	/**< feedback pattern when turn off silent mode */
	FEEDBACK_PATTERN_BT_CONNECTED,       	/**< feedback pattern when connecting with bluetooth */
	FEEDBACK_PATTERN_BT_DISCONNECTED,    	/**< feedback pattern when disconnecting with bluetooth */
	FEEDBACK_PATTERN_BT_PAIRING,         	/**< feedback pattern when starting pare with bluetooth */
	FEEDBACK_PATTERN_BT_WAITING,         	/**< feedback pattern when pairing bluetooth */
	FEEDBACK_PATTERN_SCREEN_CAPTURE,     	/**< feedback pattern when screen capture */
	FEEDBACK_PATTERN_LIST_REORDER,       	/**< feedback pattern when list reorder */
	FEEDBACK_PATTERN_LIST_SLIDER,        	/**< feedback pattern when list slider sweep */
	FEEDBACK_PATTERN_VOLUME_KEY,         	/**< feedback pattern when pressed volume key */
	FEEDBACK_PATTERN_MMS,                	/**< feedback pattern when message state changed to MMS */
	FEEDBACK_PATTERN_HOURLY_ALERT,       	/**< feedback pattern when alert every hour on the hour */

	FEEDBACK_PATTERN_SAFETY_ALERT,       	/**< feedback pattern when safety alert from phone */
	FEEDBACK_PATTERN_ACCIDENT_DETECT,    	/**< feedback pattern when accident detect */
	FEEDBACK_PATTERN_SEND_SOS_MESSAGE,   	/**< feedback pattern when sending SOS message */
	FEEDBACK_PATTERN_END_SOS_MESSAGE,    	/**< feedback pattern when finishing SOS message */
	FEEDBACK_PATTERN_EMERGENCY_BUZZER,   	/**< feedback pattern when occurs emergency buzzer */
	FEEDBACK_PATTERN_SAFETY_LOW_POWER,   	/**< this pattern is a low power ringtone */
	FEEDBACK_PATTERN_CMAS,               	/**< feedback pattern when alerting commercial mobile */
	FEEDBACK_PATTERN_SPEED_UP,           	/**< feedback pattern when advising user to speed up */
	FEEDBACK_PATTERN_SLOW_DOWN,          	/**< feedback pattern when advising user to slow down */
	FEEDBACK_PATTERN_KEEP_THIS_PACE,     	/**< feedback pattern when advising user to keep this pace */
	FEEDBACK_PATTERN_GOAL_ACHIEVED,      	/**< feedback pattern when alerting goal achieved */
	FEEDBACK_PATTERN_EXERCISE_COUNT,     	/**< feedback pattern when changing count number */
	FEEDBACK_PATTERN_START_CUE,          	/**< feedback pattern when starting cue */
	FEEDBACK_PATTERN_HEALTH_PACE,        	/**< feedback pattern when alerting health pace */
	FEEDBACK_PATTERN_INACTIVE_TIME,      	/**< feedback pattern when alerting inactive time */
	FEEDBACK_PATTERN_MEASURING_SUCCESS,  	/**< feedback pattern when measuring is succeeded */
	FEEDBACK_PATTERN_MEASURING_FAILURE,  	/**< feedback pattern when measuring is failed */
	FEEDBACK_PATTERN_UV_PROCESSING,      	/**< feedback pattern when UV is processing */
	FEEDBACK_PATTERN_SHEALTH_START,      	/**< feedback pattern when starting s-health */
	FEEDBACK_PATTERN_SHEALTH_PAUSE,      	/**< feedback pattern when pausing s-health */
	FEEDBACK_PATTERN_SHEALTH_STOP,       	/**< feedback pattern when stoping s-health */
	FEEDBACK_PATTERN_3RD_APPLICATION,    	/**< feedback pattern when alert 3rd event */

	FEEDBACK_PATTERN_END,

	/* START : Will be removed */
	FEEDBACK_PATTERN_TOUCH_TAP = FEEDBACK_PATTERN_TAP,
	FEEDBACK_PATTERN_TOUCH_MULTI_TAP = FEEDBACK_PATTERN_MULTI_TAP,
	FEEDBACK_PATTERN_TOUCH_KEY = FEEDBACK_PATTERN_KEY0,
	FEEDBACK_PATTERN_TOUCH_HOLD = FEEDBACK_PATTERN_HOLD,
	FEEDBACK_PATTERN_REACTIVE_ALERT = FEEDBACK_PATTERN_SMART_ALERT,
	FEEDBACK_PATTERN_VIBRATION = FEEDBACK_PATTERN_VIBRATION_ON,
	FEEDBACK_PATTERN_SOUND_ON = FEEDBACK_PATTERN_SILENT_OFF,
	FEEDBACK_PATTERN_SHUTTER = FEEDBACK_PATTERN_SCREEN_CAPTURE,
	FEEDBACK_PATTERN_SLIDER_SWEEP = FEEDBACK_PATTERN_LIST_SLIDER,
	FEEDBACK_PATTERN_SAFETY_ASSISTANCE = FEEDBACK_PATTERN_SEND_SOS_MESSAGE,
	FEEDBACK_PATTERN_BT_PARING = FEEDBACK_PATTERN_BT_PAIRING,
	FEEDBACK_PATTERN_CONNECTED = FEEDBACK_PATTERN_BT_CONNECTED,
	FEEDBACK_PATTERN_DISCONNECTED = FEEDBACK_PATTERN_BT_DISCONNECTED,
	FEEDBACK_PATTERN_SUCCESS = FEEDBACK_PATTERN_MEASURING_SUCCESS,
	FEEDBACK_PATTERN_FAILURE = FEEDBACK_PATTERN_MEASURING_FAILURE,
	/* END : Will be removed */

} feedback_pattern_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_H__
