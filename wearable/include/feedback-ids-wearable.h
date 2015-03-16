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


#ifndef __FEEDBACK_IDS_WEARABLE_H__
#define __FEEDBACK_IDS_WEARABLE_H__

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
 * @brief Enumerations of the system pre-defined patterns for feedback interface
 * @details
 * Each feedback pattern can have separate media files of each types.
 * But Depending on vendor design, pattern may not have any type of file.
 *
 */
typedef enum
{
	FEEDBACK_PATTERN_WEARABLE_NONE = -1,

	FEEDBACK_PATTERN_WEARABLE_SIP_BACKSPACE = FEEDBACK_PATTERN_END, /**< feedback pattern when touch backspace key */
	FEEDBACK_PATTERN_WEARABLE_SIP_FUNCTION, 			/**< feedback pattern when touch function key */
	FEEDBACK_PATTERN_WEARABLE_SIP_FJKEY,    			/**< feedback pattern when touch F,J key */
	FEEDBACK_PATTERN_WEARABLE_MAX_CHARACTER,			/**< feedback pattern when max character */

	FEEDBACK_PATTERN_WEARABLE_MESSAGE_ON_CALL, 		/**< feedback pattern when incoming a message on call */
	FEEDBACK_PATTERN_WEARABLE_EMAIL_ON_CALL,   		/**< feedback pattern when incoming an email on call */
	FEEDBACK_PATTERN_WEARABLE_WAKEUP_ON_CALL,  		/**< feedback pattern when alert wake up call on call */
	FEEDBACK_PATTERN_WEARABLE_SCHEDULE_ON_CALL,		/**< feedback pattern when alert schedule alarm on call */
	FEEDBACK_PATTERN_WEARABLE_TIMER_ON_CALL,   		/**< feedback pattern when alert timer on call */
	FEEDBACK_PATTERN_WEARABLE_GENERAL_ON_CALL, 		/**< feedback pattern when alert general event on call */
	FEEDBACK_PATTERN_WEARABLE_SMART_ALERT,     		/**< feedback pattern when alert noti on motion */

	FEEDBACK_PATTERN_WEARABLE_CHARGERCONN_ON_CALL,	/**< feedback pattern when connecting charger on call */
	FEEDBACK_PATTERN_WEARABLE_CHARGING_ERROR_ON_CALL,/**< feedback pattern when occuring charging error on call */
	FEEDBACK_PATTERN_WEARABLE_FULLCHARGED_ON_CALL,	/**< feedback pattern when full charged on call */
	FEEDBACK_PATTERN_WEARABLE_LOWBATT_ON_CALL,    	/**< feedback pattern when low battery on call */
	FEEDBACK_PATTERN_WEARABLE_CALLCONNECT,        	/**< feedback pattern when connecting call */
	FEEDBACK_PATTERN_WEARABLE_DISCALLCONNECT,     	/**< feedback pattern when disconnecting call */
	FEEDBACK_PATTERN_WEARABLE_OUTGOING_CALL,      	/**< feedback pattern when connected outgoing call */
	FEEDBACK_PATTERN_WEARABLE_MINUTEMINDER,       	/**< feedback pattern when minute minder */
	FEEDBACK_PATTERN_WEARABLE_BT_PAIRING,         	/**< feedback pattern when starting pare with bluetooth */
	FEEDBACK_PATTERN_WEARABLE_BT_WAITING,         	/**< feedback pattern when pairing bluetooth */
	FEEDBACK_PATTERN_WEARABLE_SCREEN_CAPTURE,     	/**< feedback pattern when screen capture */
	FEEDBACK_PATTERN_WEARABLE_MMS,                	/**< feedback pattern when message state changed to MMS */
	FEEDBACK_PATTERN_WEARABLE_HOURLY_ALERT,       	/**< feedback pattern when alert every hour on the hour */

	FEEDBACK_PATTERN_WEARABLE_SAFETY_ALERT,       	/**< feedback pattern when safety alert from phone */
	FEEDBACK_PATTERN_WEARABLE_ACCIDENT_DETECT,    	/**< feedback pattern when accident detect */
	FEEDBACK_PATTERN_WEARABLE_SEND_SOS_MESSAGE,   	/**< feedback pattern when sending SOS message */
	FEEDBACK_PATTERN_WEARABLE_END_SOS_MESSAGE,    	/**< feedback pattern when finishing SOS message */
	FEEDBACK_PATTERN_WEARABLE_EMERGENCY_BUZZER,   	/**< feedback pattern when occurs emergency buzzer */
	FEEDBACK_PATTERN_WEARABLE_SAFETY_LOW_POWER,   	/**< this pattern is a low power ringtone */
	FEEDBACK_PATTERN_WEARABLE_CMAS,               	/**< feedback pattern when alerting commercial mobile */
	FEEDBACK_PATTERN_WEARABLE_SPEED_UP,           	/**< feedback pattern when advising user to speed up */
	FEEDBACK_PATTERN_WEARABLE_SLOW_DOWN,          	/**< feedback pattern when advising user to slow down */
	FEEDBACK_PATTERN_WEARABLE_KEEP_THIS_PACE,     	/**< feedback pattern when advising user to keep this pace */
	FEEDBACK_PATTERN_WEARABLE_GOAL_ACHIEVED,      	/**< feedback pattern when alerting goal achieved */
	FEEDBACK_PATTERN_WEARABLE_EXERCISE_COUNT,     	/**< feedback pattern when changing count number */
	FEEDBACK_PATTERN_WEARABLE_START_CUE,          	/**< feedback pattern when starting cue */
	FEEDBACK_PATTERN_WEARABLE_HEALTH_PACE,        	/**< feedback pattern when alerting health pace */
	FEEDBACK_PATTERN_WEARABLE_INACTIVE_TIME,      	/**< feedback pattern when alerting inactive time */
	FEEDBACK_PATTERN_WEARABLE_MEASURING_SUCCESS,  	/**< feedback pattern when measuring is succeeded */
	FEEDBACK_PATTERN_WEARABLE_MEASURING_FAILURE,  	/**< feedback pattern when measuring is failed */
	FEEDBACK_PATTERN_WEARABLE_UV_PROCESSING,      	/**< feedback pattern when UV is processing */
	FEEDBACK_PATTERN_WEARABLE_SHEALTH_START,      	/**< feedback pattern when starting s-health */
	FEEDBACK_PATTERN_WEARABLE_SHEALTH_PAUSE,      	/**< feedback pattern when pausing s-health */
	FEEDBACK_PATTERN_WEARABLE_SHEALTH_STOP,       	/**< feedback pattern when stoping s-health */
	FEEDBACK_PATTERN_WEARABLE_3RD_APPLICATION,    	/**< feedback pattern when alert 3rd event */

	FEEDBACK_PATTERN_WEARABLE_END,

} feedback_pattern_wearable_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_WEARABLE_H__
