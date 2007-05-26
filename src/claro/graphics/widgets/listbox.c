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

void listbox_new_row_handle( object_t *obj, event_t *event );
void listbox_remove_row_handle( object_t *obj, event_t *event );

void listbox_widget_inst_create( object_t *object );
void listbox_widget_inst_realize( object_t *object );

claro_define_type_partial( listbox_widget, list_widget, listbox_widget_inst_create, listbox_widget_inst_realize, NULL, NULL );

void listbox_widget_inst_create( object_t *object )
{
	list_widget_init( object, 1, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( object, "new_row", listbox_new_row_handle );
	object_addhandler( object, "remove_row", listbox_remove_row_handle );
}

void listbox_widget_inst_realize( object_t *object )
{
	cgraphics_listbox_widget_create( WIDGET(object) );
}

object_t *listbox_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( listbox_widget_type, parent );
	
	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

list_item_t *listbox_append_row( object_t *listbox, char *text )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	return list_widget_row_append( listbox, 0, text );
}

list_item_t *listbox_insert_row( object_t *listbox, int pos, char *text )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	return list_widget_row_insert( listbox, 0, pos, text );
}

void listbox_new_row_handle( object_t *obj, event_t *event )
{
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	cgraphics_listbox_new_row( WIDGET(obj), item );
}

void listbox_move_row( object_t *listbox, list_item_t *item, int row )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	list_widget_row_move( listbox, item, row );
}

void listbox_remove_row( object_t *listbox, list_item_t *item )
{
	assert_valid_listbox_widget( listbox, "listbox" );
	list_widget_row_remove( listbox, item );
}

void listbox_remove_row_handle( object_t *obj, event_t *event )
{
	listbox_widget_t *lw = (listbox_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	if ( lw->selected == item )
	{
		lw->selected = NULL;
		event_send( OBJECT(lw), "selected", "p", "row", NULL );
	}
	
	cgraphics_listbox_remove_row( WIDGET(obj), item );
}

list_item_t *listbox_get_selected( object_t *obj )
{
	listbox_widget_t *lw = (listbox_widget_t *)obj;
	assert_valid_listbox_widget( obj, "obj" );
	return lw->selected;
}

int listbox_get_rows( object_t *obj )
{
	list_widget_t *lw = (list_widget_t *)obj;
	assert_valid_listbox_widget( obj, "obj" );
	return LIST_LENGTH( &lw->items );
}

void listbox_select_item( object_t *obj, list_item_t *item )
{
	assert_valid_listbox_widget( obj, "obj" );
	assert_valid_list_item( item, "item" );
	
	cgraphics_listbox_select_row( WIDGET(obj), item );
}
