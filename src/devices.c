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
#include <glib.h>

#include "devices.h"
#include "common.h"
#include "log.h"

#define DD_LIST_PREPEND(a, b)		\
	a = g_list_prepend(a, b)
#define DD_LIST_APPEND(a, b)		\
	a = g_list_append(a, b)
#define DD_LIST_REMOVE(a, b)		\
	a = g_list_remove(a, b)
#define DD_LIST_FOREACH(head, elem, node)	\
	for (elem = head; elem && ((node = elem->data) != NULL); elem = elem->next, node = NULL)

typedef GList dd_list;
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
		_D("[%s] initialize", str_type[dev->type]);
		if (dev->init)
			dev->init();
	}
}

void devices_exit(void)
{
	dd_list *elem;
	const struct device_ops *dev;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		_D("[%s] deinitialize", str_type[dev->type]);
		if (dev->exit)
			dev->exit();
	}
}

void devices_play(int pattern)
{
	dd_list *elem;
	const struct device_ops *dev;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		if (dev->play)
			dev->play(pattern);
	}
}

void devices_stop(void)
{
	dd_list *elem;
	const struct device_ops *dev;

	DD_LIST_FOREACH(dev_head, elem, dev) {
		if (dev->stop)
			dev->stop();
	}
}
