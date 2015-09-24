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


#ifndef __FEEDBACK_H__
#define __FEEDBACK_H__

#include <tizen.h>
#include <tizen_error.h>
#include "feedback-ids.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file feedback.h
 * @brief This file contains the feedback API
 */

/**
 * @addtogroup CAPI_SYSTEM_FEEDBACK_MODULE
 * @{
 */

/**
 * @brief Enumerations of error codes for the Feedback API.
 * @since_tizen 2.4
 */
typedef enum
{
    FEEDBACK_ERROR_NONE                = TIZEN_ERROR_NONE,                /**< Successful */
    FEEDBACK_ERROR_OPERATION_FAILED    = TIZEN_ERROR_NOT_PERMITTED,       /**< Operation not permitted */
    FEEDBACK_ERROR_INVALID_PARAMETER   = TIZEN_ERROR_INVALID_PARAMETER,   /**< Invalid parameter */
    FEEDBACK_ERROR_NOT_SUPPORTED       = TIZEN_ERROR_NOT_SUPPORTED,       /**< Not supported in this device */
    FEEDBACK_ERROR_PERMISSION_DENIED   = TIZEN_ERROR_PERMISSION_DENIED,   /**< Permission denied */
    FEEDBACK_ERROR_NOT_INITIALIZED     = TIZEN_ERROR_FEEDBACK | 0x01,     /**< Not initialized */
} feedback_error_e;

/**
 * @brief Initializes feedback API.
 *
 * @since_tizen 2.4
 *
 * @remarks
 * If this function is not called in advance, other function will return #FEEDBACK_ERROR_NOT_INITIALIZED.
 * And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 * If you don't have the haptic privilege, this function initializes only sound.
 * It does not return any error in this case.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE               Successful
 *
 * @post feedback_deinitialize()
 *
 * @see feedback_deinitialize()
 */
int feedback_initialize(void);

/**
 * @brief Deinitializes feedback API.
 * @details This function must be called when feedback functions are no longer needed.
 *
 * @since_tizen 2.4
 *
 * @remarks
 * If you don't want to use feedback anymore, you need to deinitialize with this function.
 * And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 * If you don't have the haptic privilege, this function deinitializes only sound.
 * It does not return any error in this case.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE                 Successful
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED      Not initialized
 *
 * @pre feedback_initialize()
 *
 * @see feedback_initialize()
 */
int feedback_deinitialize(void);

/**
 * @brief Plays various types of reactions that are pre-defined.
 * @details
 * This functon can be used to react to pre-defined actions. \n
 * It play various types of system pre-defined media or vibration patterns.
 *
 * @since_tizen 2.4
 *
 * @remarks
 * Currently, there are two types of reactions: sound and vibration. \n
 * Depending on the settings, some types cannot operate.
 * For example, when set to silent mode, the device doesn't produce any sound.
 * If to play one of devices is successful, this function regards as success.
 * And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 * If you don't have the haptic privilege, it only works sound operation.
 * It does not return any error in this case.
 *
 * @param[in] pattern   The pre-defined pattern
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE                 Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED     Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED        Not supported device
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED      Not initialized
 *
 * @pre feedback_initialize()
 */
int feedback_play(feedback_pattern_e pattern);

/**
 *
 * @brief Plays specific type of reactions that are pre-defined.
 * @details
 * This function can be used to react to pre-defined actions. \n
 * It play specific type of system pre-defined pattern.
 *
 * @since_tizen 2.4
 *
 * @remarks
 * Currently, there are two types of reactions: sound and vibration. \n
 * Depending on the settings, some types cannot operate.
 * For example, when set to silent mode, the device doesn't produce any sound.
 * And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 * If you don't have the haptic privilege, it returns FEEDBACK_ERROR_PERMISSION_DENIED error.
 *
 * @param[in] type      The pattern type
 * @param[in] pattern   The pre-defined pattern
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE                 Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED     Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED        Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED      Not initialized
 *
 * @pre feedback_initialize()
 */
int feedback_play_type(feedback_type_e type, feedback_pattern_e pattern);

/**
 * @brief Stop various types of reactions
 * @details
 * This functon can be used to stop react to pre-defined actions. \n
 * It stops system pre-defined vibration patterns.
 *
 * @since_tizen 2.4
 *
 * @remarks
 * This function does not support to stop media sound actions. \n
 * In this case, it will return FEEDBACK_ERROR_NOT_SUPPORTED error.
 * And for controlling haptic device, the privilege should be set to, %http://tizen.org/privilege/haptic.
 * If you don't have the haptic privilege, it only works sound operation.
 * It does not return any error in this case.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE                 Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED     Operation not permitted
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED        Not supported device
 * @retval #FEEDBACK_ERROR_PERMISSION_DENIED    Permission denied
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED      Not initialized
 *
 * @pre feedback_initialize()
 */
int feedback_stop(void);

/**
 * @brief Checks if the pattern is supported
 * @details
 * This functon can be used to check if a specific pattern is supported.
 *
 * @since_tizen 2.4
 *
 * @param[in] type      The pattern type
 * @param[in] pattern   The pre-defined pattern
 * @param[out] status   True means the pattern is supported, otherwise not supported.
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE                 Successful
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED     Operation not permitted
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER    Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED        Not supported device
 * @retval #FEEDBACK_ERROR_NOT_INITIALIZED      Not initialized
 *
 * @pre feedback_initialize()
 */
int feedback_is_supported_pattern(feedback_type_e type, feedback_pattern_e pattern, bool *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_H__
