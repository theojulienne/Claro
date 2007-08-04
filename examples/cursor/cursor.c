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


#include <claro/base.h>
#include <claro/graphics.h>

static GHashTable * types = NULL;
static object_t * w, *c;

static void combo_selected(object_t *combo, event_t *event) 
{
    list_item_t *li = combo_get_selected(combo);
    if(li != 0)
    {
        int cursor = GPOINTER_TO_INT(g_hash_table_lookup(types, li->data[0]));
        widget_set_cursor(w, cursor);    
    }               
}

static void window_closed(object_t *window, event_t *event) 
{
    claro_shutdown();
}

#define COMBO_APPEND(combo, item) combo_append_row(combo, #item); g_hash_table_insert(types, (void*)#item, (void*)item) 

int main(int argc, char *argv[])
{
	claro_base_init();
	claro_graphics_init();

    types = g_hash_table_new(g_str_hash, g_str_equal);
    	
	log_fd_set_level(CL_DEBUG, stderr);
	
	clog(CL_INFO, "%s running using Claro!", __FILE__);
	
	w = window_widget_create(0, new_bounds(100, 100, 230, 100), 0);
	object_addhandler(w, "destroy", window_closed);
	window_set_title(w, "Cursor Types");
	
	c = combo_widget_create(w, new_bounds(10, 10, 210, -1), 0);
	object_addhandler(c, "selected", combo_selected);
	
    list_item_t * item = COMBO_APPEND(c, cCursorNormal);
    combo_select_item(c, item);
    COMBO_APPEND(c, cCursorTextEdit);
    COMBO_APPEND(c, cCursorWait);
    COMBO_APPEND(c, cCursorPoint);
    
    
	window_show(w);
	window_focus(w);
	
	block_heap_loginfo();
	
	claro_loop();
    
    g_hash_table_destroy(types);    
	
	return 0;
}

