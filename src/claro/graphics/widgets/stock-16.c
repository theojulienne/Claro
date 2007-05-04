/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */

#include <claro/graphics.h>
#include <claro/graphics/platform.h>

struct stock_img_
{
	const char * id;
	const unsigned char * img;
	int len;
};

#include "stock-items-16.c"

static void val_free(void * v)
{
	object_t * object = (object_t*)v;
	if(!object->destroy_pending)
		event_send( object, "destroy", "" );
	printf("%s: %p\n", __FUNCTION__, v);
}

static hashtable_t * stock_imgs = NULL;

static void stock_free(object_t * obj, event_t * event)
{
	hashtable_destroy(stock_imgs);
}

void _claro_menu_stock_init()
{
	stock_imgs = hashtable_str_create(NUM_STOCK_ITEMS, FALSE, val_free);
	object_addhandler(claro, "destroy", stock_free);
	
	int i;
	for(i = 0;i < NUM_STOCK_ITEMS;i++)
	{	
		image_t * img = image_load_inline_png( claro, stock_raw_[i].img, stock_raw_[i].len);
		hashtable_insert(stock_imgs, (void*)stock_raw_[i].id, img, FALSE);
	}
}

image_t * _stock_menu_get_image(const char * stock_id)
{
	return (image_t*) hashtable_search(stock_imgs, (void*) stock_id);
}

void _stock_menu_add_image(const char * stock_id, image_t * img)
{
	hashtable_insert(stock_imgs, (void*)stock_id, img, FALSE);
}

