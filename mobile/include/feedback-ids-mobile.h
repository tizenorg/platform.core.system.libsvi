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


#ifndef __FEEDBACK_IDS_MOBILE_H__
#define __FEEDBACK_IDS_MOBILE_H__

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file feedback-ids-mobile.h
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
	FEEDBACK_TYPE_MOBILE_LED = FEEDBACK_TYPE_END,
	FEEDBACK_TYPE_MOBILE_END,
} feedback_mobile_type_e;

/**
 * @brief Enumerations of the system pre-defined patterns for feedback interface
 * @details
 * Each feedback pattern can have separate media files of each types.
 * But Depending on vendor design, pattern may not have any type of file.
 *
 */
typedef enum
{
	FEEDBACK_PATTERN_MOBILE_NONE = -1,

	FEEDBACK_PATTERN_MOBILE_SIP_BACKSPACE = 2, /**< feedback pattern when touch backspace key */
	FEEDBACK_PATTERN_MOBILE_SIP_FUNCTION = 3, 			/**< feedback pattern when touch function key */
	FEEDBACK_PATTERN_MOBILE_SIP_FJKEY = 4,    			/**< feedback pattern when touch F,J key */
	FEEDBACK_PATTERN_MOBILE_MAX_CHARACTER = 5,			/**< feedback pattern when max character */

	FEEDBACK_PATTERN_MOBILE_MULTI_TAP = 20,    			/**< feedback pattern when multi touch */

	FEEDBACK_PATTERN_MOBILE_MESSAGE_ON_CALL = 24, 		/**< feedback pattern when incoming a message on call */
	FEEDBACK_PATTERN_MOBILE_EMAIL_ON_CALL = 26,   		/**< feedback pattern when incoming an email on call */
	FEEDBACK_PATTERN_MOBILE_WAKEUP_ON_CALL = 28,  		/**< feedback pattern when alert wake up call on call */
	FEEDBACK_PATTERN_MOBILE_SCHEDULE_ON_CALL = 30,		/**< feedback pattern when alert schedule alarm on call */
	FEEDBACK_PATTERN_MOBILE_TIMER_ON_CALL = 32,   		/**< feedback pattern when alert timer on call */
	FEEDBACK_PATTERN_MOBILE_GENERAL_ON_CALL = 34, 		/**< feedback pattern when alert general event on call */
	FEEDBACK_PATTERN_MOBILE_SMART_ALERT = 35,     		/**< feedback pattern when alert noti on motion */

	FEEDBACK_PATTERN_MOBILE_CHARGERCONN_ON_CALL = 39,	/**< feedback pattern when connecting charger on call */
	FEEDBACK_PATTERN_MOBILE_CHARGING_ERROR_ON_CALL = 41,/**< feedback pattern when occuring charging error on call */
	FEEDBACK_PATTERN_MOBILE_FULLCHARGED_ON_CALL = 43,	/**< feedback pattern when full charged on call */
	FEEDBACK_PATTERN_MOBILE_LOWBATT_ON_CALL = 45,    	/**< feedback pattern when low battery on call */
	FEEDBACK_PATTERN_MOBILE_LOCK_SWIPE = 48,         	/**< feedback pattern when lock swipe */
	FEEDBACK_PATTERN_MOBILE_UNLOCK_SWIPE = 49,       	/**< feedback pattern when unlock swipe*/
	FEEDBACK_PATTERN_MOBILE_GEOMETRIC_LOCK = 50,     	/**< feedback pattern when lock by geometric effect */
	FEEDBACK_PATTERN_MOBILE_CALLCONNECT = 51,        	/**< feedback pattern when connecting call */
	FEEDBACK_PATTERN_MOBILE_DISCALLCONNECT = 52,     	/**< feedback pattern when disconnecting call */
	FEEDBACK_PATTERN_MOBILE_OUTGOING_CALL = 53,      	/**< feedback pattern when connected outgoing call */
	FEEDBACK_PATTERN_MOBILE_MINUTEMINDER = 54,       	/**< feedback pattern when minute minder */
	FEEDBACK_PATTERN_MOBILE_BT_PAIRING = 59,         	/**< feedback pattern when starting pare with bluetooth */
	FEEDBACK_PATTERN_MOBILE_BT_WAITING = 60,         	/**< feedback pattern when pairing bluetooth */
	FEEDBACK_PATTERN_MOBILE_SCREEN_CAPTURE = 61,     	/**< feedback pattern when screen capture */
	FEEDBACK_PATTERN_MOBILE_MMS = 65,                	/**< feedback pattern when message state changed to MMS */
	FEEDBACK_PATTERN_MOBILE_HOURLY_ALERT,       	/**< feedback pattern when alert every hour on the hour */

	FEEDBACK_PATTERN_MOBILE_SAFETY_ALERT,       	/**< feedback pattern when safety alert from phone */
	FEEDBACK_PATTERN_MOBILE_ACCIDENT_DETECT,    	/**< feedback pattern when accident detect */
	FEEDBACK_PATTERN_MOBILE_SEND_SOS_MESSAGE,   	/**< feedback pattern when sending SOS message */
	FEEDBACK_PATTERN_MOBILE_END_SOS_MESSAGE,    	/**< feedback pattern when finishing SOS message */
	FEEDBACK_PATTERN_MOBILE_EMERGENCY_BUZZER,   	/**< feedback pattern when occurs emergency buzzer */
	FEEDBACK_PATTERN_MOBILE_SAFETY_LOW_POWER,   	/**< this pattern is a low power ringtone */
	FEEDBACK_PATTERN_MOBILE_CMAS,               	/**< feedback pattern when alerting commercial mobile */

	FEEDBACK_PATTERN_MOBILE_END,

} feedback_pattern_mobile_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_MOBILE_H__
