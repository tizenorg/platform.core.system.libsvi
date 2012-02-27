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

#include <tet_api.h>
#include <svi.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_ApplicationFW_svi_init_func_01(void);

static int handle = 0;

enum {
	POSITIVE_TC_IDX = 0x01,
};

struct tet_testlist tet_testlist[] = {
	{ utc_ApplicationFW_svi_init_func_01, POSITIVE_TC_IDX },
	{ NULL, 0},
};

static void startup(void)
{
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of svi_init()
 */
static void utc_ApplicationFW_svi_init_func_01(void)
{
	int r = 0;


	r = svi_init(&handle);

	if (r) {
		tet_infoline("svi_init() failed in positive test case");
		tet_result(TET_FAIL);
		return;
	}
	tet_result(TET_PASS);

	svi_fini(handle);
}

