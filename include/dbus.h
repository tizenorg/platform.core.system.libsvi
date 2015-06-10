/*
 * feedback
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


#ifndef __DBUS_H__
#define __DBUS_H__

#include <dbus/dbus.h>

/*
 * Device daemon
 */
#define DEVICED_BUS_NAME                    "org.tizen.system.deviced"
#define DEVICED_OBJECT_PATH                 "/Org/Tizen/System/DeviceD"
#define DEVICED_INTERFACE_NAME              DEVICED_BUS_NAME
/* Led service: play/stop led operations about led */
#define DEVICED_PATH_LED                    DEVICED_OBJECT_PATH"/Led"
#define DEVICED_INTERFACE_LED               DEVICED_INTERFACE_NAME".Led"
/* Haptic service: operatioins about haptic */
#define DEVICED_PATH_HAPTIC                 DEVICED_OBJECT_PATH"/Haptic"
#define DEVICED_INTERFACE_HAPTIC            DEVICED_INTERFACE_NAME".haptic"

struct dbus_byte {
	const unsigned char *data;
	int size;
};

int dbus_method_sync(const char *dest, const char *path,
		const char *interface, const char *method,
		const char *sig, char *param[]);
#endif
