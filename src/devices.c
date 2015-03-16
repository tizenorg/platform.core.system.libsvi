/*
 * deviced
 *
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


#include <stdio.h>

#include "devices.h"
#include "log.h"

static dd_list *dev_head;

void add_device(const struct device_ops *dev)
{
	DD_LIST_APPEND(dev_head, (struct device_ops*)dev);
}

void remove_device(const struct device_ops *dev)
{
	DD_LIST_REMOVE(dev_head, (struct device_ops*)dev);
}

const struct device_ops *find_device(int type)
{
	dd_list *elem;
	const struct device_ops *dev;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		if (dev->type == type)
			return dev;
	}
	return NULL;
}

void devices_init(void)
{
	dd_list *elem;
	const struct device_ops *dev;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		_D("[%s] initialize", dev->name);
		if (dev->init)
			dev->init();
	}
}

void devices_exit(void)
{
	dd_list *elem;
	const struct device_ops *dev;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		_D("[%s] deinitialize", dev->name);
		if (dev->exit)
			dev->exit();
	}
}

int devices_play(int pattern)
{
	dd_list *elem;
	const struct device_ops *dev;
	int ret, prev = -EPERM;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		if (dev->play) {
			ret = dev->play(pattern);
			if ((prev < 0 && ret == 0) ||
			    (prev == 0 && ret < 0))
				prev = 0;
			else if ((prev < 0 && ret == -ENOTSUP) ||
			         (prev == -ENOTSUP && ret < 0))
				prev = -ENOTSUP;
			else
				prev = ret;
		}
	}

	return prev;
}

int devices_stop(void)
{
	dd_list *elem;
	const struct device_ops *dev;
	int ret = -ENOTSUP;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		if (dev->stop)
			ret = dev->stop();
	}

	return ret;
}
