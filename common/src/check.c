/*
 * libfeedback
 * Copyright (c) 2015 Samsung Electronics Co., Ltd.
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

#include "feedback.h"
#include "profiles.h"
#include "log.h"


static const char *common_str_type[] = {
	"FEEDBACK_TYPE_NONE",
	"FEEDBACK_TYPE_SOUND",
	"FEEDBACK_TYPE_VIBRATION",
};

static const char *common_str_pattern[] = {
	"FEEDBACK_PATTERN_TAP",
	"FEEDBACK_PATTERN_SIP",
	"",
	"",
	"",
	"",
	"FEEDBACK_PATTERN_KEY0",
	"FEEDBACK_PATTERN_KEY1",
	"FEEDBACK_PATTERN_KEY2",
	"FEEDBACK_PATTERN_KEY3",
	"FEEDBACK_PATTERN_KEY4",
	"FEEDBACK_PATTERN_KEY5",
	"FEEDBACK_PATTERN_KEY6",
	"FEEDBACK_PATTERN_KEY7",
	"FEEDBACK_PATTERN_KEY8",
	"FEEDBACK_PATTERN_KEY9",
	"FEEDBACK_PATTERN_KEY_STAR",
	"FEEDBACK_PATTERN_KEY_SHARP",
	"FEEDBACK_PATTERN_KEY_BACK",
	"FEEDBACK_PATTERN_HOLD",
	"",
	"FEEDBACK_PATTERN_HW_TAP",
	"FEEDBACK_PATTERN_HW_HOLD",

	"FEEDBACK_PATTERN_MESSAGE",
	"",
	"FEEDBACK_PATTERN_EMAIL",
	"",
	"FEEDBACK_PATTERN_WAKEUP",
	"",
	"FEEDBACK_PATTERN_SCHEDULE",
	"",
	"FEEDBACK_PATTERN_TIMER",
	"",
	"FEEDBACK_PATTERN_GENERAL",
	"",
	"",

	"FEEDBACK_PATTERN_POWERON",
	"FEEDBACK_PATTERN_POWEROFF",
	"FEEDBACK_PATTERN_CHARGERCONN",
	"",
	"FEEDBACK_PATTERN_CHARGING_ERROR",
	"",
	"FEEDBACK_PATTERN_FULLCHARGED",
	"",
	"FEEDBACK_PATTERN_LOWBATT",
	"",
	"FEEDBACK_PATTERN_LOCK",
	"FEEDBACK_PATTERN_UNLOCK",
	"",
	"",
	"",
	"",
	"",
	"",
	"",
	"FEEDBACK_PATTERN_VIBRATION_ON",
	"FEEDBACK_PATTERN_SILENT_OFF",
	"FEEDBACK_PATTERN_BT_CONNECTED",
	"FEEDBACK_PATTERN_BT_DISCONNECTED",
	"",
	"",
	"",
	"FEEDBACK_PATTERN_LIST_REORDER",
	"FEEDBACK_PATTERN_LIST_SLIDER",
	"FEEDBACK_PATTERN_VOLUME_KEY",
};

static bool common_get_always_alert_case(int type, int pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_SIP:
	case FEEDBACK_PATTERN_TIMER:
		if (CHECK_VIBRATION(type))
			return true;
		break;
	case FEEDBACK_PATTERN_WAKEUP:
		return true;
	default:
		break;
	}
	return false;
}

static bool common_get_always_off_case(int type, int pattern)
{
	switch (pattern) {
	case FEEDBACK_PATTERN_TAP ... FEEDBACK_PATTERN_SIP:
	case FEEDBACK_PATTERN_HOLD ... FEEDBACK_PATTERN_HW_HOLD:
		if (CHECK_SOUND(type) && !is_touch_sndstatus())
			return true;
		break;
	case FEEDBACK_PATTERN_KEY0 ... FEEDBACK_PATTERN_KEY_BACK:
		if (CHECK_SOUND(type) && !is_keytone_sndstatus())
			return true;
		break;
	default:
		break;
	}
	return false;
}

static int common_get_strength_type(int type, int pattern)
{
	if (CHECK_SOUND(type)) {
		if (pattern == FEEDBACK_PATTERN_TAP)
			return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_TOUCH;
		else if (pattern >= FEEDBACK_PATTERN_KEY0 && pattern <= FEEDBACK_PATTERN_KEY_BACK)
			return VOLUME_TYPE_SYSTEM|VOLUME_GAIN_DIALER;
		else if (pattern == FEEDBACK_PATTERN_VOLUME_KEY)
			return VOLUME_TYPE_RINGTONE;
		else
			return VOLUME_TYPE_SYSTEM;
	} else if (CHECK_VIBRATION(type)) {
		return DEFAULT_VIB_LEVEL * HAPTIC_FEEDBACK_STEP;
	}

	return -EINVAL;
}

static const struct profile_ops common_profile_ops = {
	.profile               = "common",
	.get_always_alert_case = common_get_always_alert_case,
	.get_always_off_case   = common_get_always_off_case,
	.get_strength_type     = common_get_strength_type,
	.max_type              = FEEDBACK_TYPE_END,
	.max_pattern           = FEEDBACK_PATTERN_END,
	.str_type              = common_str_type,
	.str_pattern           = common_str_pattern,
};

PROFILE_OPS_REGISTER(&common_profile_ops);
