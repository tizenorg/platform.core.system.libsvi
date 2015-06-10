/*
 *  libfeedback
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

#include <stdio.h>
#include "feedback.h"

int main(int argc, char* argv[])
{
	char buf[4096] = {0,};
	char *temp = NULL;
	int val, input;
	int a, b;

	val = feedback_initialize();
	if (val != FEEDBACK_ERROR_NONE) {
		printf("feedback_initialize error : %d\n", val);
		return -1;
	}

	while(1)
	{
		printf("Which do you want to work play(0) or change the path(1)?");
		scanf("%d", &a);
		if (a == 0) {
			printf("Please input value : ");
			scanf("%d", &a);
			if (a == -1)
				break;
			val = feedback_play(a);
			printf("ret value : %d\n", val);	
		} else if (a == 1) {
			printf("Please input type(sound:0,vib:1), enum, new path  : ");
			scanf("%d %d %s", &a, &b, buf);
			val = feedback_get_resource_path( (a == 0) ? FEEDBACK_TYPE_SOUND : FEEDBACK_TYPE_VIBRATION, b, &temp);
			printf("Prev : %s", temp);
			free(temp);
			val = feedback_set_resource_path( (a == 0) ? FEEDBACK_TYPE_SOUND : FEEDBACK_TYPE_VIBRATION, b, buf);
			val = feedback_get_resource_path( (a == 0) ? FEEDBACK_TYPE_SOUND : FEEDBACK_TYPE_VIBRATION, b, &temp);
			printf("Curr : %s", temp);
			free(temp);
		} else {
			printf("Input error!!");
		}
	}

	val = feedback_deinitialize();
	if (val != FEEDBACK_ERROR_NONE) {
		printf("feedback_initialize error : %d\n", val);
		return -1;
	}

	return 0;
}
