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


#ifndef __FEEDBACK_INTERNAL_H__
#define __FEEDBACK_INTERNAL_H__

#include <tizen_error.h>
#include "feedback-ids.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @file feedback-internal.h
 * @brief This file contains the feedback internal API
 */

#define FEEDBACK_SUCCEEDED(n)                ((n) == FEEDBACK_ERROR_NONE)
#define FEEDBACK_FAILED(n)                   ((n) != FEEDBACK_ERROR_NONE)

/**
 *
 * @brief Plays specific type of reactions that are pre-defined.
 * @details
 * This function can be used to react to pre-defined actions. \n
 * It play specific type of system pre-defined pattern.
 *
 * @since_tizen 2.3
 *
 * @remarks
 * Currently, there are two types of reactions: sound and vibration.
 *
 * @param[in] type string      The pattern type
 * @param[in] pattern string   The pre-defined pattern
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE               Successful
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED      Not supported device
 */
int feedback_play_type_by_name(char *type, char *pattern);

/**
 * @brief Gets the file path of resource for the given feedback type and pattern.
 * @details
 * Depending on the type of each pattern resouorce has a different format. \n
 * Currently, System supports two pattern types. \n
 * #FEEDBACK_TYPE_SOUND type uses .wav format. \n
 * #FEEDBACK_TYPE_VIBRATION type uses .ivt format. \n
 * If the given pattern doesn't have a file for the type, @a path will return NULL.
 *
 * @since_tizen 2.3
 *
 * @remarks @a path must be released with free() by you.
 *
 * @param[in]  type      The pattern type
 * @param[in]  pattern   The pre-defined pattern
 * @param[out] path      The file path of resource for feedback type and pattern
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE               Successful
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED   Operation failed
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED      Not supported device
 */
int feedback_get_resource_path(feedback_type_e type, feedback_pattern_e pattern, char **path);

/**
 * @brief Sets the new file path of resource for the given feedback type and pattern.
 * @details
 * Depending on the type of each pattern resouorce has a different format. \n
 * Currently, System supports two pattern types. \n
 * #FEEDBACK_TYPE_SOUND type uses .wav format. \n
 * #FEEDBACK_TYPE_VIBRATION type uses .ivt format. \n
 * If the given pattern doesn't have a file for the type, @a path will return NULL.
 *
 * @since_tizen 2.3
 *
 * @param[in]  type      The pattern type
 * @param[in]  pattern   The pre-defined pattern
 * @param[in]  path      The new file path of resource for feedback type and pattern
 *
 * @return 0 on success, otherwise a negative error value.
 * @retval #FEEDBACK_ERROR_NONE               Successful
 * @retval #FEEDBACK_ERROR_INVALID_PARAMETER  Invalid parameter
 * @retval #FEEDBACK_ERROR_OPERATION_FAILED   Operation failed
 * @retval #FEEDBACK_ERROR_NOT_SUPPORTED      Not supported device
 */
int feedback_set_resource_path(feedback_type_e type, feedback_pattern_e pattern, char *path);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif //__FEEDBACK_INTERNAL_H__
