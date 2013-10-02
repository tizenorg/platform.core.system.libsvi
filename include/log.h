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


#ifndef __LOG_H__
#define __LOG_H__

#define FEATURE_FEEDBACK_DLOG

#ifdef FEATURE_FEEDBACK_DLOG
	#define LOG_TAG "FEEDBACK"
	#include <dlog.h>
	#define FEEDBACK_LOG(fmt, args...)       SLOGD(fmt, ##args)
	#define FEEDBACK_ERROR(fmt, args...)     SLOGE(fmt, ##args)
#else
	#define FEEDBACK_LOG(x, ...)
	#define FEEDBACK_ERROR(x, ...)
#endif

#endif //__LOG_H__
