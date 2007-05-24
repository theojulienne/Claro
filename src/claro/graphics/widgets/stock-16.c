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
}

static GHashTable * stock_imgs = NULL;

static void stock_free(object_t * obj, event_t * event)
{
	g_hash_table_unref(stock_imgs);
}

void _claro_menu_stock_init()
{
	stock_imgs = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, val_free);
	object_addhandler(claro, "destroy", stock_free);
	
	int i;
	for(i = 0;i < NUM_STOCK_ITEMS;i++)
	{	
		image_t * img = image_load_inline_png( claro, stock_raw_[i].img, stock_raw_[i].len);
		g_hash_table_insert(stock_imgs, (void*)stock_raw_[i].id, (void*)img);
	}
}

image_t * _stock_menu_get_image(const char * stock_id)
{
	return (image_t*) g_hash_table_lookup(stock_imgs, (void*) stock_id);
}

void _stock_menu_add_image(const char * stock_id, image_t * img)
{
	g_hash_table_insert(stock_imgs, (void*)stock_id, (void*)img);
}

