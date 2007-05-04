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

void _claro_menu_stock_init();
image_t * _stock_menu_get_image(const char * stock_id);
void _stock_menu_add_image(const char * stock_id, image_t * img);

void _claro_tb_stock_init();
image_t * _stock_tb_get_image(const char * stock_id);
void _stock_tb_add_image(const char * stock_id, image_t * img);

void claro_stock_init()
{
    _claro_menu_stock_init();
    _claro_tb_stock_init();
}

image_t * stock_get_image(const char * stock_id, int size)
{
    if(size == cStockMenu)
        return _stock_menu_get_image(stock_id);
    else if(size == cStockTb)
        return _stock_tb_get_image(stock_id);
    else
        return NULL;    
}

void stock_add_image(const char * stock_id, int size, image_t * img)
{
	if(size == cStockMenu)
        return _stock_menu_add_image(stock_id, img);
    else if(size == cStockTb)
        return _stock_tb_add_image(stock_id, img);
    //add a warning here    
}

