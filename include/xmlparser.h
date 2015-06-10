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


#ifndef __XMLPARSER_H__
#define __XMLPARSER_H__

#include <libxml/parser.h>

struct xmlData {
    char *label;
    char *data;
	int size;
};

enum xmlType {
	XML_LABEL = 0,
	XML_DATA,
};

#define VIBRATION_STR		"vibration"
#define SOUND_STR			"sound"

xmlDocPtr xml_open(const char *xml);
void xml_close(xmlDocPtr doc);

xmlNodePtr xml_find(xmlDocPtr doc, const char *label, const xmlChar* expr);
struct xmlData *xml_parse(xmlDocPtr doc, xmlNodePtr cur);
int xml_decode_data(struct xmlData *data);
int xml_save(xmlDocPtr doc, const char *path);
void xml_free(struct xmlData* data);

#endif //__XMLPARSER_H__
