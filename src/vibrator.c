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


#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <limits.h>
#include <vconf.h>
#include <sys/stat.h>

#include "feedback-ids.h"
#include "common.h"
#include "log.h"
#include "devices.h"
#include "xmlparser.h"
#include "dbus.h"

#define DEFAULT_VIB_LEVEL			3
#define HAPTIC_FEEDBACK_STEP		20 /**< feedback max / slider step */

#define HAPTIC_DEVICE				0

enum haptic_priority
{
	HAPTIC_PRIORITY_MIN = 0,
	HAPTIC_PRIORITY_MIDDLE,
	HAPTIC_PRIORITY_HIGH,
};

enum haptic_iteration
{
	HAPTIC_ITERATION_ONCE = 1,
	HAPTIC_ITERATION_INFINITE = 256,
};

#define VIBRATION_XML               "/usr/share/feedback/vibration.xml"

#define METHOD_OPEN                 "OpenDevice"
#define METHOD_CLOSE                "CloseDevice"
#define METHOD_VIBRATE_BUFFER       "VibrateBuffer"
#define METHOD_VIBRATE_MONOTONE     "VibrateMonotone"
#define METHOD_STOP                 "StopDevice"

#define DEFAULT_DURATION            100

static int vibstatus;
static int vib_level;
static int noti_level;

static unsigned int v_handle;
static xmlDocPtr v_doc;

static char haptic_file[FEEDBACK_PATTERN_END][NAME_MAX];

static int haptic_open(void)
{
	char *arr[1];
	char buf_index[32];

	snprintf(buf_index, sizeof(buf_index), "%d", HAPTIC_DEVICE);
	arr[0] = buf_index;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_OPEN,
			"i", arr);
}

static int haptic_close(unsigned int handle)
{
	char *arr[1];
	char buf_handle[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_CLOSE,
			"u", arr);
}

static int haptic_vibrate_monotone(unsigned int handle,
								int duration,
								int feedback,
								int priority)
{
	char *arr[4];
	char buf_handle[32];
	char buf_duration[32];
	char buf_feedback[32];
	char buf_priority[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;
	snprintf(buf_duration, sizeof(buf_duration), "%d", duration);
	arr[1] = buf_duration;
	snprintf(buf_feedback, sizeof(buf_feedback), "%d", feedback);
	arr[2] = buf_feedback;
	snprintf(buf_priority, sizeof(buf_priority), "%d", priority);
	arr[3] = buf_priority;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_VIBRATE_MONOTONE,
			"uiii", arr);
}

static int haptic_vibrate_buffer(unsigned int handle,
								const unsigned char *buffer,
								int size,
								int iteration,
								int feedback,
								int priority)
{
	char *arr[6];
	char buf_handle[32];
	char buf_iteration[32];
	char buf_feedback[32];
	char buf_priority[32];
	struct dbus_byte bytes;

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;
	bytes.size = size;
	bytes.data = buffer;
	arr[2] = (char*)&bytes;
	snprintf(buf_iteration, sizeof(buf_iteration), "%d", iteration);
	arr[3] = buf_iteration;
	snprintf(buf_feedback, sizeof(buf_feedback), "%d", feedback);
	arr[4] = buf_feedback;
	snprintf(buf_priority, sizeof(buf_priority), "%d", priority);
	arr[5] = buf_priority;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_VIBRATE_BUFFER,
			"uayiii", arr);
}

static int haptic_vibrate_stop(unsigned int handle)
{
	char *arr[1];
	char buf_handle[32];

	snprintf(buf_handle, sizeof(buf_handle), "%u", handle);
	arr[0] = buf_handle;

	return dbus_method_sync(DEVICED_BUS_NAME, DEVICED_PATH_HAPTIC,
			DEVICED_INTERFACE_HAPTIC, METHOD_STOP,
			"u", arr);
}

static unsigned char* convert_file_to_buffer(const char *file_name, int *size)
{
	FILE *pf;
	long file_size;
	unsigned char *pdata = NULL;

	if (!file_name)
		return NULL;

	/* Get File Stream Pointer */
	pf = fopen(file_name, "rb");
	if (!pf) {
		_E("fopen failed : %s", strerror(errno));
		return NULL;
	}

	if (fseek(pf, 0, SEEK_END))
		goto error;

	file_size = ftell(pf);
	if (fseek(pf, 0, SEEK_SET))
		goto error;

	if (file_size < 0)
		goto error;

	pdata = (unsigned char*)malloc(file_size);
	if (!pdata)
		goto error;

	if (fread(pdata, 1, file_size, pf) != file_size)
		goto err_free;

	fclose(pf);
	*size = file_size;
	return pdata;

err_free:
	free(pdata);

error:
	fclose(pf);

	_E("failed to convert file to buffer (%s)", strerror(errno));
	return NULL;
}

static void feedback_vib_cb(keynode_t *key, void* data)
{
	vib_level = vconf_keynode_get_int(key);
}

static void feedback_noti_cb(keynode_t *key, void* data)
{
	noti_level = vconf_keynode_get_int(key);
}

static int get_priority(feedback_pattern_e pattern)
{
	if (pattern >= FEEDBACK_PATTERN_TAP && pattern <= FEEDBACK_PATTERN_HW_HOLD)
		return HAPTIC_PRIORITY_MIN;

	return HAPTIC_PRIORITY_MIDDLE;
}

static int get_haptic_level(feedback_pattern_e pattern)
{
	int level;

	if (pattern >= FEEDBACK_PATTERN_MESSAGE && pattern <= FEEDBACK_PATTERN_SMART_ALERT)
		level = noti_level * HAPTIC_FEEDBACK_STEP;
	else
		level = vib_level * HAPTIC_FEEDBACK_STEP;

	_D("Call status : %d, pattern : %s, level : %d", callstatus, str_pattern[pattern], level);
	if (callstatus == VCONFKEY_CALL_VOICE_ACTIVE
	    || callstatus == VCONFKEY_CALL_VIDEO_ACTIVE) {
		// if call status is ON, vibration magnitude is 20%
		level = 20;
		_D("level changed : %d", level);
	}

	return level;
}

static bool get_always_alert_case(feedback_pattern_e pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_KEY0 ... FEEDBACK_PATTERN_KEY_BACK:
	case FEEDBACK_PATTERN_HOLD:
		break;
	case FEEDBACK_PATTERN_SIP:
	case FEEDBACK_PATTERN_SIP_BACKSPACE:
	case FEEDBACK_PATTERN_SIP_FUNCTION:
	case FEEDBACK_PATTERN_SIP_FJKEY:
		return true;
	case FEEDBACK_PATTERN_TIMER:
	case FEEDBACK_PATTERN_TIMER_ON_CALL:
	case FEEDBACK_PATTERN_WAKEUP:
	case FEEDBACK_PATTERN_WAKEUP_ON_CALL:
		return true;
	case FEEDBACK_PATTERN_MESSAGE_ON_CALL:
	case FEEDBACK_PATTERN_EMAIL_ON_CALL:
	case FEEDBACK_PATTERN_GENERAL_ON_CALL:
		if (alert_callstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_MESSAGE:
	case FEEDBACK_PATTERN_EMAIL:
	case FEEDBACK_PATTERN_3RD_APPLICATION:
	case FEEDBACK_PATTERN_SMART_ALERT:
	case FEEDBACK_PATTERN_SEND_SOS_MESSAGE:
	case FEEDBACK_PATTERN_END_SOS_MESSAGE:
	case FEEDBACK_PATTERN_CMAS:
	case FEEDBACK_PATTERN_OUTGOING_CALL:
	case FEEDBACK_PATTERN_MMS:
	case FEEDBACK_PATTERN_HOURLY_ALERT:
		return true;
	case FEEDBACK_PATTERN_SPEED_UP:
	case FEEDBACK_PATTERN_SLOW_DOWN:
	case FEEDBACK_PATTERN_KEEP_THIS_PACE:
	case FEEDBACK_PATTERN_GOAL_ACHIEVED:
	case FEEDBACK_PATTERN_EXERCISE_COUNT:
	case FEEDBACK_PATTERN_START_CUE:
		/* except mute case */
		if (is_sound_mode() ||  vibstatus)
			return true;
		break;
	case FEEDBACK_PATTERN_CHARGERCONN_ON_CALL:
	case FEEDBACK_PATTERN_CHARGING_ERROR_ON_CALL:
	case FEEDBACK_PATTERN_LOWBATT_ON_CALL:
		/* no matter sound profile */
		return true;
	default:
		break;
	}
	return false;
}

static bool get_always_off_case(feedback_pattern_e pattern)
{
	return false;
}

static int get_xml_data(xmlDocPtr doc, feedback_pattern_e pattern, struct xmlData **data)
{
	xmlNodePtr cur;
	struct xmlData *retData;

	cur = xml_find(doc, VIBRATION_STR, (const xmlChar*)str_pattern[pattern]);
	/* This pattern does not have sound file to play */
	if (cur == NULL)
		return -ENOENT;

	retData = xml_parse(doc, cur);
	if (retData == NULL) {
		_E("xml_parse fail");
		return -EPERM;
	}

	*data = retData;
	return 0;
}

static void release_xml_data(struct xmlData *data)
{
	if (data == NULL)
		return;

	xml_free(data);
}

static void vibrator_init(void)
{
	int ret;

	/* xml Init */
	v_doc = xml_open(VIBRATION_XML);
	if (v_doc == NULL) {
		_E("xml_open(%s) fail", VIBRATION_XML);
		return;
	}

	/* Vibration Init */
	ret = haptic_open();
	if (ret < 0) {
		_E("haptic_open ==> FAIL!! : %d", ret);
		xml_close(v_doc);
		v_doc = NULL;
		return;
	}

	/* Set vibration handle */
	v_handle = (unsigned int)ret;

	/* check vib_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, &vib_level) < 0)
		_W("VCONFKEY_FEEDBACK_VIBRATION_LEVEL_INT ==> FAIL!!");

	/* check noti_level */
	if (vconf_get_int(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, &noti_level) < 0)
		_W("VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT ==> FAIL!!");

	/* add watch for status value */
	vconf_notify_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb, NULL);
	vconf_notify_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb, NULL);
}

static void vibrator_exit(void)
{
	int ret;

	/* remove watch */
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_TOUCH_FEEDBACK_VIBRATION_LEVEL_INT, feedback_vib_cb);
	vconf_ignore_key_changed(VCONFKEY_SETAPPL_NOTI_VIBRATION_LEVEL_INT, feedback_noti_cb);

	if (v_handle) {
		ret = haptic_close(v_handle);
		if (ret < 0)
			_E("haptic_close is failed : %d", ret);
		v_handle = 0;
	}

	if (v_doc) {
		xml_close(v_doc);
		v_doc = NULL;
	}
}

static int vibrator_play(feedback_pattern_e pattern)
{
	int ret;

	if (!v_handle || !v_doc) {
		_E("Not initialize");
		return -EPERM;
	}

	if (vconf_get_bool(VCONFKEY_SETAPPL_VIBRATION_STATUS_BOOL, &vibstatus) < 0) {
		_D("fail to get vibration status, will work as turning off");
		vibstatus = 0;
	}

	if (vibstatus == 0 && !get_always_alert_case(pattern))  {
		_D("Vibration condition is OFF (vibstatus : %d)", vibstatus);
		return 0;
	}

	if (vibstatus && get_always_off_case(pattern)) {
		_D("Vibration always off condition");
		return 0;
	}

	ret = haptic_vibrate_monotone(v_handle, DEFAULT_DURATION,
			get_haptic_level(pattern), get_priority(pattern));
	if (ret < 0) {
		_E("haptic_vibrate_monotone is failed");
		return -EPERM;
	}

	return 0;
}

static int vibrator_stop(void)
{
	int ret;

	if (!v_handle || !v_doc) {
		_E("Not initialize");
		return -EPERM;
	}

	/* stop haptic device */
	ret = haptic_vibrate_stop(v_handle);
	if (ret < 0) {
		_E("haptic_vibrate_stop is failed");
		return -EPERM;
	}

	return 0;
}

static int vibrator_get_path(feedback_pattern_e pattern, char *buf, unsigned int buflen)
{
	const char *cur_path;

	assert(buf != NULL && buflen > 0);

	cur_path = haptic_file[pattern];
	if (*cur_path) {
		_E("This pattern(%s) in vibrator type is not supported to play", str_pattern[pattern]);
		snprintf(buf, buflen, "NULL");
		return -ENOENT;
	}

	snprintf(buf, buflen, "%s", cur_path);
	return 0;
}

static int vibrator_set_path(feedback_pattern_e pattern, char *path)
{
	struct stat buf;
	char *ppath;

	/*
	 * check the path is valid
	 * if path is null, below operation is ignored
	 */
	if (path && stat(path, &buf)) {
		_E("%s is not presents", path);
		return -errno;
	}

	ppath = haptic_file[pattern];

	/* if path is NULL, this pattern set to default file */
	if (path)
		snprintf(ppath, NAME_MAX, "%s", path);
	else
		memset(ppath, 0, NAME_MAX);

	_D("The file of pattern(%s) is changed to [%s]", str_pattern[pattern], path);
	return 0;
}

static const struct device_ops vibrator_device_ops = {
	.type = FEEDBACK_TYPE_VIBRATION,
	.init = vibrator_init,
	.exit = vibrator_exit,
	.play = vibrator_play,
	.stop = vibrator_stop,
	.get_path = vibrator_get_path,
	.set_path = vibrator_set_path,
};

DEVICE_OPS_REGISTER(&vibrator_device_ops);
