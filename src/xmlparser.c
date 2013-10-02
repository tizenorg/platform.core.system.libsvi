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
#include <assert.h>
#include <glib.h>

#include "xmlparser.h"
#include "log.h"

static const char* data_str[] = {
	[XML_LABEL] = "label",
	[XML_DATA]  = "data",
};

xmlDocPtr xml_open(const char *xml)
{
	xmlDocPtr doc;

	doc = xmlReadFile(xml, NULL, 0);
	if (doc == NULL) {
		_E("xmlReadFile fail");
		return NULL;
	}

	return doc;
}

void xml_close(xmlDocPtr doc)
{
	xmlFreeDoc(doc);
}

static int xml_compare(xmlDocPtr doc, xmlNodePtr cur, const xmlChar* expr)
{
	xmlNodePtr node;
	xmlChar *key;
	int r;

	assert(doc);
	assert(cur);
	assert(expr);

	for (node = cur->children; node != NULL; node = node->next) {
		if (xmlStrcmp(node->name, (const xmlChar*)data_str[XML_LABEL]))
			continue;

		key = xmlNodeListGetString(doc, node->children, 1);
		r = xmlStrcmp(key, expr);
		xmlFree(key);

		if (r == 0)
			return 1;

		break;
	}

	return 0;
}

xmlNodePtr xml_find(xmlDocPtr doc, const xmlChar* expr)
{
	xmlNodePtr root;
	xmlNodePtr cur;

	assert(doc);
	assert(expr);

	root = xmlDocGetRootElement(doc);
	if (root == NULL) {
		_E("xmlDocGetRootElement fail");
		return NULL;
	}

	for (cur = root->children; cur != NULL; cur = cur->next) {
		if (xmlStrcmp(cur->name, (const xmlChar*)VIBRATION))
			continue;

		if (!xml_compare(doc, cur, expr))
			continue;

		return cur;
	}

	return NULL;
}

struct xmlData *xml_parse(xmlDocPtr doc, xmlNodePtr cur)
{
	xmlNodePtr node;
	struct xmlData *data;
	char *b64_data;
	unsigned int len;

	assert(doc);
	assert(cur);

	data = (struct xmlData*)malloc(sizeof(struct xmlData));
	if (data == NULL) {
		_E("out of memory");
		return NULL;
	}

	memset(data, 0, sizeof(struct xmlData));
	for (node = cur->children; node != NULL; node = node->next) {
		if (!xmlStrcmp(node->name, (const xmlChar*)data_str[XML_LABEL])) {
			data->label = (char*)xmlNodeListGetString(doc, node->children, 1);
			_D("label : %s", data->label);
		} else if (!xmlStrcmp(node->name, (const xmlChar*)data_str[XML_DATA])) {
			b64_data = (char *)xmlNodeListGetString(doc, node->children, 1);
			if (b64_data != NULL) {
				_D("b64_data : %s", b64_data);
				data->data = g_base64_decode(b64_data, &len);
				xmlFree(b64_data);
			}
		}
	}

	return data;
}

int xml_save(xmlDocPtr doc, const char *path)
{
	int r;

	assert(doc);
	assert(path);

	r = xmlSaveFile(path, doc);
	if (r < 0) {
		_E("xmlSaveFile fail");
		return -1;
	}

	return 0;
}

void xml_free(struct xmlData* data)
{
	assert(data);

	free(data->label);
	free(data->data);
	free(data);
}
