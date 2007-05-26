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

void toolbar_new_icon_handle( object_t *obj, event_t *event );
void toolbar_remove_icon_handle( object_t *obj, event_t *event );

void toolbar_widget_inst_realize( object_t *object );
void toolbar_widget_inst_create( object_t *object );

claro_define_widget_partial( toolbar, toolbar_widget_inst_create, toolbar_widget_inst_realize, NULL, NULL );

void toolbar_widget_inst_create( object_t *object )
{
	list_widget_init( object, 3, CLIST_TYPE_PTR, CLIST_TYPE_STRING, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( object, "new_row", toolbar_new_icon_handle );
	object_addhandler( object, "remove_row", toolbar_remove_icon_handle );
}

void toolbar_widget_inst_realize( object_t *object )
{
	cgraphics_toolbar_widget_create( WIDGET(object) );
}

object_t *toolbar_widget_create( object_t *parent, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( toolbar_widget_type, parent );
	
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

/*
list_item_t *toolbar_append_popup_item( object_t *toolbar, list_item_t *parent, image_t *image, char *title, char *tooltip )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_only_list_item( parent, "parent" );
	assert_only_image( image, "image" );
	
	return list_widget_row_append( toolbar, parent, image, title, tooltip );
}

list_item_t *toolbar_insert_popup_item( object_t *toolbar, list_item_t *parent, int pos, image_t *image, char *title, char *tooltip )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_only_list_item( parent, "parent" );
	assert_only_image( image, "image" );
	
	return list_widget_row_insert( toolbar, parent, pos, image, title, tooltip );
}
*/

void toolbar_set_item_menu( object_t *toolbar, list_item_t *item, object_t *menu )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_valid_list_item( item, "item" );
	assert_only_menu_widget( menu, "menu" );
	
	item->menu = menu;
	
	cgraphics_toolbar_set_menu( toolbar, item );
}

list_item_t *toolbar_append_icon( object_t *toolbar, image_t *image, char *title, char *tooltip )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_only_image( image, "image" );
	
	return list_widget_row_append( toolbar, 0, image, title, tooltip );
}

list_item_t *toolbar_insert_icon( object_t *toolbar, int pos, image_t *image, char *title, char *tooltip )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_only_image( image, "image" );
	
	return list_widget_row_insert( toolbar, 0, pos, image, title, tooltip );
}

list_item_t *toolbar_append_separator( object_t *toolbar )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	
	return list_widget_row_append( toolbar, 0, 0, 0, 0 );
}

list_item_t *toolbar_insert_separator( object_t *toolbar, int pos )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	
	return list_widget_row_insert( toolbar, 0, pos, 0, 0, 0 );
}

void toolbar_new_icon_handle( object_t *obj, event_t *event )
{
	list_widget_t *lw = (list_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	if ( item->parent == &lw->items )
		cgraphics_toolbar_new_icon( WIDGET(obj), item );
	/*else
		cgraphics_toolbar_new_popup_item( WIDGET(obj), item );*/
}

void toolbar_move_icon( object_t *toolbar, list_item_t *item, int row )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_move( toolbar, item, row );
}

void toolbar_remove_icon( object_t *toolbar, list_item_t *item )
{
	assert_valid_toolbar_widget( toolbar, "toolbar" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_remove( toolbar, item );
}

void toolbar_remove_icon_handle( object_t *obj, event_t *event )
{
	list_widget_t *lw = (list_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	if ( item->parent == &lw->items )
		cgraphics_toolbar_remove_icon( WIDGET(obj), item );
	/*else
		cgraphics_toolbar_remove_popup_item( WIDGET(obj), item );*/
}

int toolbar_item_count( object_t *obj )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	assert_valid_toolbar_widget( obj, "obj" );
	
	return LIST_LENGTH( &lw->items );
}
