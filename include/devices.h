/*
 * libfeedback

 * Copyright (c) 2012 - 2013 Samsung Electronics Co., Ltd.
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


#ifndef __DEVICES_H__
#define __DEVICES_H__

struct device_ops {
	int type;
	void (*init) (void);
	void (*exit) (void);
	int (*play) (int);
	int (*stop) (void);
	int (*get_path) (int, char *, unsigned int);
	int (*set_path) (int, char *);
};

void devices_init(void);
void devices_exit(void);
void devices_play(int pattern);
void devices_stop(void);

#ifndef __CONSTRUCTOR__
#define __CONSTRUCTOR__ __attribute__ ((constructor))
#endif

#ifndef __DESTRUCTOR__
#define __DESTRUCTOR__ __attribute__ ((destructor))
#endif

#define DEVICE_OPS_REGISTER(dev)	\
static void __CONSTRUCTOR__ module_init(void)	\
{	\
	add_device(dev);	\
}

void add_device(const struct device_ops *dev);
void remove_device(const struct device_ops *dev);
const struct device_ops *find_device(int type);

#endif
