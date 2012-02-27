/*
 *  libsvi
 *
 * Copyright (c) 2000 - 2011 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Contact: Seokkyu Jang <seokkyu.jang@samsung.com>
 * Contact: Sangil Yoon <si83.yoon@samsung.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef __SVI_LOG_H__
#define __SVI_LOG_H__

#define FEATURE_SVI_DLOG

#ifdef FEATURE_SVI_DLOG
    #define LOG_TAG "SVI"
    #include <dlog.h>
    #define SVILOG(fmt, args...)  SLOGD("<%s:%4d> " fmt "\n", __func__, __LINE__, ##args)
#else
    #define SVILOG(x, ...)
#endif

#endif //__SVI_LOG_H__
