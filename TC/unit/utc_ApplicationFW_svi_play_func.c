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
#include <stdio.h>

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;

static void utc_ApplicationFW_svi_play_func_01(void);
static void utc_ApplicationFW_svi_play_func_02(void);

static int handle = 0;
enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_ApplicationFW_svi_play_func_01, POSITIVE_TC_IDX },
	{ utc_ApplicationFW_svi_play_func_02, NEGATIVE_TC_IDX },
	{ NULL, 0},
};

static void startup(void)
{
	int r;
	char *err;

	r = svi_init(&handle);

	if (r) {
		err = "Cannot init svi";
                tet_infoline(err);
                tet_delete(POSITIVE_TC_IDX, err);
                tet_delete(NEGATIVE_TC_IDX, err);
	}
}

static void cleanup(void)
{
	svi_fini(handle);
}

/**
 * @brief Positive test case of svi_play()
 */
static void utc_ApplicationFW_svi_play_func_01(void)
{
	int r = 0;

	r = svi_play(handle, SVI_VIB_NONE, SVI_SND_NONE);

	if (r) {
		tet_infoline("svi_play() failed in positive test case");
		tet_result(TET_FAIL);
		return;
	}

	r = svi_play(handle, SVI_VIB_TOUCH_TOUCH, SVI_SND_TOUCH_TOUCH1);

	if (r) {
		tet_infoline("svi_play() failed in positive test case");
		tet_result(TET_FAIL);
		return;
	}

	r = svi_play(handle, SVI_VIB_OPERATION_VIBRATION, SVI_SND_TOUCH_KEY00);

	if (r) {
		tet_infoline("svi_play() failed in positive test case");
		tet_result(TET_FAIL);
		return;
	}

	r = svi_play(handle, SVI_VIB_OPERATION_POWER_ON, SVI_SND_TOUCH_KEY_STAR);

	if (r) {
		tet_infoline("svi_play() failed in positive test case");
		tet_result(TET_FAIL);
		return;
	}

	tet_result(TET_PASS);
}

/**
 * @brief Negative test case of ug_init svi_play()
 */
static void utc_ApplicationFW_svi_play_func_02(void)
{
	int r = 0;

	r = svi_play(handle, -100, SVI_SND_NONE);

	if (r != -1) {
		tet_infoline("svi_play() failed in negative test case");
		tet_result(TET_FAIL);
		return;
	}

	r = svi_play(handle, SVI_VIB_NONE, -100);

	if (r != -1) {
		tet_infoline("svi_play() failed in negative test case");
		tet_result(TET_FAIL);
		return;
	}


	tet_result(TET_PASS);
}
