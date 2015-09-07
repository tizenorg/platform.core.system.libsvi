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
 * @since_tizen 2.4
 */
typedef enum
{
	FEEDBACK_PATTERN_NONE = -1,
	FEEDBACK_PATTERN_MOBILE_NONE = FEEDBACK_PATTERN_NONE,
	FEEDBACK_PATTERN_WEARABLE_NONE = FEEDBACK_PATTERN_NONE,

	FEEDBACK_PATTERN_TAP = 0,      					/**< Feedback pattern when general touch */
	FEEDBACK_PATTERN_SIP,          					/**< Feedback pattern when touch text key */
	FEEDBACK_PATTERN_MOBILE_SIP_BACKSPACE = 2, 			/**< Feedback pattern when touch backspace key for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SIP_BACKSPACE = 2, 			/**< Feedback pattern when touch backspace key for wearable device */
	FEEDBACK_PATTERN_MOBILE_SIP_FUNCTION = 3, 			/**< Feedback pattern when touch function key for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SIP_FUNCTION = 3, 			/**< Feedback pattern when touch function key for wearable device */
	FEEDBACK_PATTERN_MOBILE_SIP_FJKEY = 4,    			/**< Feedback pattern when touch F,J key for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SIP_FJKEY = 4,    			/**< Feedback pattern when touch F,J key for wearable device */
	FEEDBACK_PATTERN_MOBILE_MAX_CHARACTER = 5,			/**< Feedback pattern when max character for mobile device */
	FEEDBACK_PATTERN_WEARABLE_MAX_CHARACTER = 5,			/**< Feedback pattern when max character for wearable device */
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
	FEEDBACK_PATTERN_MOBILE_MULTI_TAP,    				/**< Feedback pattern when multi touch for mobile device */
	FEEDBACK_PATTERN_HW_TAP,  					/**< Feedback pattern when press hardware key */
	FEEDBACK_PATTERN_HW_HOLD,      					/**< Feedback pattern when holding press hardware key */

	FEEDBACK_PATTERN_MESSAGE,         				/**< Feedback pattern when incoming a message */
	FEEDBACK_PATTERN_MOBILE_MESSAGE_ON_CALL = 24, 			/**< Feedback pattern when incoming a message on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_MESSAGE_ON_CALL = 24, 		/**< Feedback pattern when incoming a message on call for wearable device */
	FEEDBACK_PATTERN_EMAIL = 25,           				/**< Feedback pattern when incoming an email */
	FEEDBACK_PATTERN_MOBILE_EMAIL_ON_CALL = 26,   			/**< Feedback pattern when incoming an email on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_EMAIL_ON_CALL = 26,   		/**< Feedback pattern when incoming an email on call for wearable device  */
	FEEDBACK_PATTERN_WAKEUP = 27,          				/**< Feedback pattern when alert wake up call */
	FEEDBACK_PATTERN_MOBILE_WAKEUP_ON_CALL = 28,  			/**< Feedback pattern when alert wake up call on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_WAKEUP_ON_CALL = 28,  		/**< Feedback pattern when alert wake up call on call for wearable device */
	FEEDBACK_PATTERN_SCHEDULE = 29,        				/**< Feedback pattern when alert schedule alarm */
	FEEDBACK_PATTERN_MOBILE_SCHEDULE_ON_CALL = 30,			/**< Feedback pattern when alert schedule alarm on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SCHEDULE_ON_CALL = 30,		/**< Feedback pattern when alert schedule alarm on call for wearable device */
	FEEDBACK_PATTERN_TIMER = 31,           				/**< Feedback pattern when alert timer */
	FEEDBACK_PATTERN_MOBILE_TIMER_ON_CALL = 32,   			/**< Feedback pattern when alert timer on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_TIMER_ON_CALL = 32,   		/**< Feedback pattern when alert timer on call for wearable device */
	FEEDBACK_PATTERN_GENERAL = 33,         				/**< Feedback pattern when alert general event */
	FEEDBACK_PATTERN_MOBILE_GENERAL_ON_CALL = 34, 			/**< Feedback pattern when alert general event on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_GENERAL_ON_CALL = 34, 		/**< Feedback pattern when alert general event on call for wearable device */
	FEEDBACK_PATTERN_MOBILE_SMART_ALERT = 35,     			/**< Feedback pattern when alert noti on motion for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SMART_ALERT = 35,     		/**< Feedback pattern when alert noti on motion for wearable device */

	FEEDBACK_PATTERN_POWERON = 36,            			/**< Feedback pattern when power on */
	FEEDBACK_PATTERN_POWEROFF,         				/**< Feedback pattern when power off */
	FEEDBACK_PATTERN_CHARGERCONN,           			/**< Feedback pattern when connecting charger */
	FEEDBACK_PATTERN_MOBILE_CHARGERCONN_ON_CALL = 39,		/**< Feedback pattern when connecting charger on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_CHARGERCONN_ON_CALL = 39,		/**< Feedback pattern when connecting charger on call for wearable device */
	FEEDBACK_PATTERN_CHARGING_ERROR = 40,     			/**< Feedback pattern when occuring charging error */
	FEEDBACK_PATTERN_MOBILE_CHARGING_ERROR_ON_CALL = 41,		/**< Feedback pattern when occuring charging error on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_CHARGING_ERROR_ON_CALL = 41,		/**< Feedback pattern when occuring charging error on call for wearable device */
	FEEDBACK_PATTERN_FULLCHARGED = 42,        			/**< Feedback pattern when full charged */
	FEEDBACK_PATTERN_MOBILE_FULLCHARGED_ON_CALL = 43,		/**< Feedback pattern when full charged on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_FULLCHARGED_ON_CALL = 43,		/**< Feedback pattern when full charged on call for wearable device */
	FEEDBACK_PATTERN_LOWBATT = 44,            			/**< Feedback pattern when low battery */
	FEEDBACK_PATTERN_MOBILE_LOWBATT_ON_CALL = 45,    		/**< Feedback pattern when low battery on call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_LOWBATT_ON_CALL = 45,    		/**< Feedback pattern when low battery on call for wearable device */
	FEEDBACK_PATTERN_LOCK = 46,               			/**< Feedback pattern when lock */
	FEEDBACK_PATTERN_UNLOCK,             				/**< Feedback pattern when unlock */
	FEEDBACK_PATTERN_MOBILE_LOCK_SWIPE,         			/**< Feedback pattern when lock swipe for mobile device */
	FEEDBACK_PATTERN_MOBILE_UNLOCK_SWIPE,       			/**< Feedback pattern when unlock swipe for mobile device */
	FEEDBACK_PATTERN_MOBILE_GEOMETRIC_LOCK,     			/**< Feedback pattern when lock by geometric effect for mobile device */
	FEEDBACK_PATTERN_MOBILE_CALLCONNECT = 51,        		/**< Feedback pattern when connecting call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_CALLCONNECT = 51,        		/**< Feedback pattern when connecting call for wearable device */
	FEEDBACK_PATTERN_MOBILE_DISCALLCONNECT = 52,     		/**< Feedback pattern when disconnecting call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_DISCALLCONNECT = 52,     		/**< Feedback pattern when disconnecting call for wearable device */
	FEEDBACK_PATTERN_MOBILE_OUTGOING_CALL = 53,      		/**< Feedback pattern when connected outgoing call for mobile device */
	FEEDBACK_PATTERN_WEARABLE_OUTGOING_CALL = 53,      		/**< Feedback pattern when connected outgoing call for wearable device */
	FEEDBACK_PATTERN_MOBILE_MINUTEMINDER = 54,       		/**< Feedback pattern when minute minder for mobile device */
	FEEDBACK_PATTERN_WEARABLE_MINUTEMINDER = 54,       		/**< Feedback pattern when minute minder for wearable device */
	FEEDBACK_PATTERN_VIBRATION_ON = 55,       			/**< Feedback pattern when turn on vibration mode */
	FEEDBACK_PATTERN_SILENT_OFF,         				/**< Feedback pattern when turn off silent mode */
	FEEDBACK_PATTERN_BT_CONNECTED,       				/**< Feedback pattern when connecting with bluetooth */
	FEEDBACK_PATTERN_BT_DISCONNECTED,    				/**< Feedback pattern when disconnecting with bluetooth */
	FEEDBACK_PATTERN_MOBILE_BT_PAIRING = 59,         		/**< Feedback pattern when starting pare with bluetooth for mobile device */
	FEEDBACK_PATTERN_WEARABLE_BT_PAIRING = 59,         		/**< Feedback pattern when starting pare with bluetooth for wearable device */
	FEEDBACK_PATTERN_MOBILE_BT_WAITING = 60,         		/**< Feedback pattern when pairing bluetooth for mobile device */
	FEEDBACK_PATTERN_WEARABLE_BT_WAITING = 60,         		/**< Feedback pattern when pairing bluetooth for wearable device */
	FEEDBACK_PATTERN_MOBILE_SCREEN_CAPTURE = 61,     		/**< Feedback pattern when screen capture for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SCREEN_CAPTURE = 61,     		/**< Feedback pattern when screen capture for wearable device */
	FEEDBACK_PATTERN_LIST_REORDER = 62,       			/**< Feedback pattern when list reorder */
	FEEDBACK_PATTERN_LIST_SLIDER,        				/**< Feedback pattern when list slider sweep */
	FEEDBACK_PATTERN_VOLUME_KEY,         				/**< Feedback pattern when pressed volume key */

	FEEDBACK_PATTERN_END,

	FEEDBACK_PATTERN_MOBILE_MMS = 65,                		/**< Feedback pattern when message state changed to MMS for mobile device */
	FEEDBACK_PATTERN_WEARABLE_MMS = 65,                		/**< Feedback pattern when message state changed to MMS for wearable device  */
	FEEDBACK_PATTERN_MOBILE_HOURLY_ALERT = 66,       		/**< Feedback pattern when alert every hour on the hour for mobile device */
	FEEDBACK_PATTERN_WEARABLE_HOURLY_ALERT = 66,       		/**< Feedback pattern when alert every hour on the hour for wearale device */

	FEEDBACK_PATTERN_MOBILE_SAFETY_ALERT = 67,       		/**< Feedback pattern when safety alert from phone for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SAFETY_ALERT = 67,       		/**< Feedback pattern when safety alert from phone for wearable device */
	FEEDBACK_PATTERN_MOBILE_ACCIDENT_DETECT = 68,    		/**< Feedback pattern when accident detect for mobile device */
	FEEDBACK_PATTERN_WEARABLE_ACCIDENT_DETECT = 68,    		/**< Feedback pattern when accident detect for wearable device */
	FEEDBACK_PATTERN_MOBILE_SEND_SOS_MESSAGE = 69,   		/**< Feedback pattern when sending SOS message for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SEND_SOS_MESSAGE = 69,   		/**< Feedback pattern when sending SOS message for wearable device */
	FEEDBACK_PATTERN_MOBILE_END_SOS_MESSAGE = 70,    		/**< Feedback pattern when finishing SOS message for mobile device */
	FEEDBACK_PATTERN_WEARABLE_END_SOS_MESSAGE = 70,    		/**< Feedback pattern when finishing SOS message for wearable device */
	FEEDBACK_PATTERN_MOBILE_EMERGENCY_BUZZER = 71,   		/**< Feedback pattern when occurs emergency buzzer for mobile device */
	FEEDBACK_PATTERN_WEARABLE_EMERGENCY_BUZZER = 71,   		/**< Feedback pattern when occurs emergency buzzer for wearable device */
	FEEDBACK_PATTERN_MOBILE_SAFETY_LOW_POWER = 72,   		/**< Feedback pattern when low power ringtone for mobile device */
	FEEDBACK_PATTERN_WEARABLE_SAFETY_LOW_POWER = 72,   		/**< Feedback pattern when low power ringtone for wearable device */
	FEEDBACK_PATTERN_MOBILE_CMAS = 73,               		/**< Feedback pattern when alerting commercial mobile */
	FEEDBACK_PATTERN_WEARABLE_CMAS = 73,               		/**< Feedback pattern when alerting commercial mobile */

	FEEDBACK_PATTERN_MOBILE_END = 74,

	FEEDBACK_PATTERN_WEARABLE_SPEED_UP = 74,           		/**< Feedback pattern when advising user to speed up for wearable device */
	FEEDBACK_PATTERN_WEARABLE_SLOW_DOWN,          			/**< Feedback pattern when advising user to slow down for wearable device  */
	FEEDBACK_PATTERN_WEARABLE_KEEP_THIS_PACE,     			/**< Feedback pattern when advising user to keep this pace for wearable device */
	FEEDBACK_PATTERN_WEARABLE_GOAL_ACHIEVED,      			/**< Feedback pattern when alerting goal achieved for wearable device */
	FEEDBACK_PATTERN_WEARABLE_EXERCISE_COUNT,     			/**< Feedback pattern when changing count number for wearable device */
	FEEDBACK_PATTERN_WEARABLE_START_CUE,          			/**< Feedback pattern when starting cue for wearable device */
	FEEDBACK_PATTERN_WEARABLE_HEALTH_PACE,        			/**< Feedback pattern when alerting health pace for wearable device */
	FEEDBACK_PATTERN_WEARABLE_INACTIVE_TIME,      			/**< Feedback pattern when alerting inactive time for wearable device */
	FEEDBACK_PATTERN_WEARABLE_MEASURING_SUCCESS,  			/**< Feedback pattern when measuring is succeeded for wearable device */
	FEEDBACK_PATTERN_WEARABLE_MEASURING_FAILURE,  			/**< Feedback pattern when measuring is failed for wearable device */
	FEEDBACK_PATTERN_WEARABLE_UV_PROCESSING,      			/**< Feedback pattern when UV is processing for wearable device */
	FEEDBACK_PATTERN_WEARABLE_SHEALTH_START,      			/**< Feedback pattern when starting s-health for wearable device */
	FEEDBACK_PATTERN_WEARABLE_SHEALTH_PAUSE,      			/**< Feedback pattern when pausing s-health for wearable device */
	FEEDBACK_PATTERN_WEARABLE_SHEALTH_STOP,       			/**< Feedback pattern when stoping s-health for wearable device */
	FEEDBACK_PATTERN_WEARABLE_3RD_APPLICATION,    			/**< Feedback pattern when alert 3rd event for wearable device */

	FEEDBACK_PATTERN_WEARABLE_END,

} feedback_pattern_e;

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_H__
