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
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_IDS_MOBILE_H__
