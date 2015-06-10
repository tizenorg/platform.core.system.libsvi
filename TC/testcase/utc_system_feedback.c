/*
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd All Rights Reserved
 * PROPRIETARY/CONFIDENTIAL
 *
 * This software is the confidential and proprietary information of SAMSUNG
 * ELECTRONICS ("Confidential Information"). You agree and acknowledge that
 * this software is owned by Samsung and you shall not disclose such
 * Confidential Information and shall use it only in accordance with the terms
 * of the license agreement you entered into with SAMSUNG ELECTRONICS. SAMSUNG
 * make no representations or warranties about the suitability of the software,
 * either express or implied, including but not limited to the implied
 * warranties of merchantability, fitness for a particular purpose, or
 * non-infringement. SAMSUNG shall not be liable for any damages suffered by
 * licensee arising out of or related to this software.
 *
 */
#include <tet_api.h>
#include <feedback.h>

#define API_NAME_FEEDBACK_INITIALIZE "feedback_initialize"
#define API_NAME_FEEDBACK_DEINITIALIZE "feedback_deinitialize"
#define API_NAME_FEEDBACK_PLAY "feedback_play"
#define API_NAME_FEEDBACK_PLAY_TYPE "feedback_play_type"
#define API_NAME_FEEDBACK_GET_RESOURCE_PATH "feedback_get_resource_path"
#define API_NAME_FEEDBACK_SET_RESOURCE_PATH "feedback_set_resource_path"

static void startup(void);
static void cleanup(void);

void (*tet_startup)(void) = startup;
void (*tet_cleanup)(void) = cleanup;


static void utc_system_feedback_initialize_p(void);
static void utc_system_feedback_deinitialize_p(void);
static void utc_system_feedback_play_p(void);
static void utc_system_feedback_play_n(void);
static void utc_system_feedback_play_type_p(void);
static void utc_system_feedback_play_type_n_1(void);
static void utc_system_feedback_play_type_n_2(void);
static void utc_system_feedback_get_resource_path_p(void);
static void utc_system_feedback_get_resource_path_n_1(void);
static void utc_system_feedback_get_resource_path_n_2(void);
static void utc_system_feedback_get_resource_path_n_3(void);
static void utc_system_feedback_set_resource_path_p(void);
static void utc_system_feedback_set_resource_path_n_1(void);
static void utc_system_feedback_set_resource_path_n_2(void);
static void utc_system_feedback_set_resource_path_n_3(void);
static void utc_system_feedback_set_resource_path_n_4(void);

enum {
	POSITIVE_TC_IDX = 0x01,
	NEGATIVE_TC_IDX,
};

struct tet_testlist tet_testlist[] = {
	{ utc_system_feedback_initialize_p, POSITIVE_TC_IDX },
	{ utc_system_feedback_deinitialize_p, POSITIVE_TC_IDX },
	{ utc_system_feedback_play_p, POSITIVE_TC_IDX },
	{ utc_system_feedback_play_n, NEGATIVE_TC_IDX },
	{ utc_system_feedback_play_type_p, POSITIVE_TC_IDX },
	{ utc_system_feedback_play_type_n_1, NEGATIVE_TC_IDX },
	{ utc_system_feedback_play_type_n_2, NEGATIVE_TC_IDX },
	{ utc_system_feedback_get_resource_path_p, POSITIVE_TC_IDX },
	{ utc_system_feedback_get_resource_path_n_1, NEGATIVE_TC_IDX },
	{ utc_system_feedback_get_resource_path_n_2, NEGATIVE_TC_IDX },
	{ utc_system_feedback_get_resource_path_n_3, NEGATIVE_TC_IDX },
	{ utc_system_feedback_set_resource_path_p, POSITIVE_TC_IDX },
	{ utc_system_feedback_set_resource_path_n_1, NEGATIVE_TC_IDX },
	{ utc_system_feedback_set_resource_path_n_2, NEGATIVE_TC_IDX },
	{ utc_system_feedback_set_resource_path_n_3, NEGATIVE_TC_IDX },
	{ utc_system_feedback_set_resource_path_n_4, NEGATIVE_TC_IDX },
	{ NULL, 0 },
};

#define SOUND_FILE_PATH "/usr/share/feedback/sound/touch/touch.ogg"
#define VIBRATION_FILE_PATH "/usr/share/deviced/HW_touch_30ms_sharp.ivt"

static void startup(void)
{
}

static void cleanup(void)
{
}

/**
 * @brief Positive test case of feedback_initialize()
 */
static void utc_system_feedback_initialize_p(void)
{
	int ret;

	ret = feedback_initialize();
	dts_check_eq(API_NAME_FEEDBACK_INITIALIZE, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Positive test case of feedback_deinitialize()
 */
static void utc_system_feedback_deinitialize_p(void)
{
	int ret;

	ret = feedback_deinitialize();
	dts_check_eq(API_NAME_FEEDBACK_DEINITIALIZE, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Positive test case of feedback_play()
 */
static void utc_system_feedback_play_p(void)
{
	int i, ret;

	for (i = 0; i < FEEDBACK_PATTERN_END; ++i) {
		dts_message(API_NAME_FEEDBACK_PLAY, "Play [%d] pattern", i);

		ret = feedback_play(i);
		dts_check_eq(API_NAME_FEEDBACK_PLAY, ret, FEEDBACK_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of feedback_play()
 */
static void utc_system_feedback_play_n(void)
{
	int ret;

	ret = feedback_play(-1);
	dts_check_ne(API_NAME_FEEDBACK_PLAY, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Positive test case of feedback_play_type()
 */
static void utc_system_feedback_play_type_p(void)
{
	int i, ret;

	for (i = 0; i < FEEDBACK_PATTERN_END; ++i) {
		dts_message(API_NAME_FEEDBACK_PLAY_TYPE, "Play [%d] pattern", i);

		ret = feedback_play_type(FEEDBACK_TYPE_SOUND, i);
		dts_check_eq(API_NAME_FEEDBACK_PLAY_TYPE, ret, FEEDBACK_ERROR_NONE, "Sound Test");

		ret = feedback_play_type(FEEDBACK_TYPE_VIBRATION, i);
		dts_check_eq(API_NAME_FEEDBACK_PLAY_TYPE, ret, FEEDBACK_ERROR_NONE, "Vibration Test");
	}
}

/**
 * @brief Negative test case of feedback_play_type()
 */
static void utc_system_feedback_play_type_n_1(void)
{
	int ret;

	ret = feedback_play_type(-1, FEEDBACK_PATTERN_TAP);
	dts_check_ne(API_NAME_FEEDBACK_PLAY_TYPE, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Negative test case of feedback_play_type()
 */
static void utc_system_feedback_play_type_n_2(void)
{
	int ret;

	ret = feedback_play_type(FEEDBACK_TYPE_SOUND, -1);
	dts_check_ne(API_NAME_FEEDBACK_PLAY_TYPE, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Positive test case of feedback_get_resource_path()
 */
static void utc_system_feedback_get_resource_path_p(void)
{
	int i, ret;
	char *str = NULL;

	for (i = 0; i < FEEDBACK_PATTERN_END; ++i) {
		dts_message(API_NAME_FEEDBACK_GET_RESOURCE_PATH, "Get [%d] pattern path", i);

		ret = feedback_get_resource_path(FEEDBACK_TYPE_SOUND, i, &str);
		dts_check_eq(API_NAME_FEEDBACK_GET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE,
				"[%d] sound file path : %s", i, str);
		free(str);
		str = NULL;

		ret = feedback_get_resource_path(FEEDBACK_TYPE_VIBRATION, i, &str);
		dts_check_eq(API_NAME_FEEDBACK_GET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE,
				"[%d] vibration file path : %s", i, str);
		free(str);
		str = NULL;
	}
}

/**
 * @brief Negative test case of feedback_get_resource_path()
 */
static void utc_system_feedback_get_resource_path_n_1(void)
{
	int ret;
	char *str = NULL;

	ret = feedback_get_resource_path(-1, FEEDBACK_PATTERN_TAP, &str);
	dts_check_ne(API_NAME_FEEDBACK_GET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Negative test case of feedback_get_resource_path()
 */
static void utc_system_feedback_get_resource_path_n_2(void)
{
	int ret;
	char *str = NULL;

	ret = feedback_get_resource_path(FEEDBACK_TYPE_SOUND, -1, &str);
	dts_check_ne(API_NAME_FEEDBACK_GET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Negative test case of feedback_get_resource_path()
 */
static void utc_system_feedback_get_resource_path_n_3(void)
{
	int ret;

	ret = feedback_get_resource_path(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_TAP, NULL);
	dts_check_ne(API_NAME_FEEDBACK_GET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Positive test case of feedback_set_resource_path()
 */
static void utc_system_feedback_set_resource_path_p(void)
{
	int i, ret;

	for (i = 0; i < FEEDBACK_PATTERN_END; ++i) {
		dts_message(API_NAME_FEEDBACK_SET_RESOURCE_PATH, "Set [%d] pattern path", i);

		ret = feedback_set_resource_path(FEEDBACK_TYPE_SOUND, i, SOUND_FILE_PATH);
		dts_check_eq(API_NAME_FEEDBACK_SET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);

		ret = feedback_set_resource_path(FEEDBACK_TYPE_VIBRATION, i, VIBRATION_FILE_PATH);
		dts_check_eq(API_NAME_FEEDBACK_SET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
	}
}

/**
 * @brief Negative test case of feedback_set_resource_path()
 */
static void utc_system_feedback_set_resource_path_n_1(void)
{
	int ret;

	ret = feedback_set_resource_path(-1, FEEDBACK_PATTERN_TAP, SOUND_FILE_PATH);
	dts_check_ne(API_NAME_FEEDBACK_SET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Negative test case of feedback_set_resource_path()
 */
static void utc_system_feedback_set_resource_path_n_2(void)
{
	int ret;

	ret = feedback_set_resource_path(FEEDBACK_TYPE_SOUND, -1, SOUND_FILE_PATH);
	dts_check_ne(API_NAME_FEEDBACK_SET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Negative test case of feedback_set_resource_path()
 */
static void utc_system_feedback_set_resource_path_n_3(void)
{
	int ret;

	ret = feedback_set_resource_path(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_TAP, NULL);
	dts_check_ne(API_NAME_FEEDBACK_SET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

/**
 * @brief Negative test case of feedback_set_resource_path()
 */
static void utc_system_feedback_set_resource_path_n_4(void)
{
	int ret;

	ret = feedback_set_resource_path(FEEDBACK_TYPE_SOUND, FEEDBACK_PATTERN_TAP, "/usr/share/no_file");
	dts_check_ne(API_NAME_FEEDBACK_SET_RESOURCE_PATH, ret, FEEDBACK_ERROR_NONE);
}

