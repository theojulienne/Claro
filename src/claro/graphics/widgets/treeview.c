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

void treeview_new_row_handle( object_t *obj, event_t *event );
void treeview_remove_row_handle( object_t *obj, event_t *event );
event_handler( treeview_edit_row_handle );

void treeview_widget_inst_create( object_t *object );
void treeview_widget_inst_realize( object_t *object );

claro_define_type_partial( treeview_widget, list_widget, treeview_widget_inst_create, treeview_widget_inst_realize, NULL, NULL );

void treeview_widget_inst_create( object_t *object )
{
	list_widget_init( object, 2, CLIST_TYPE_PTR, CLIST_TYPE_STRING );
	
	/* handle list operations */
	object_addhandler( object, "new_row", treeview_new_row_handle );
	object_addhandler( object, "remove_row", treeview_remove_row_handle );
	object_addhandler( object, "edit_row", treeview_edit_row_handle );
}


void treeview_widget_inst_realize( object_t *object )
{
	cgraphics_treeview_widget_create( WIDGET(object) );
}

object_t *treeview_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( treeview_widget_type, parent );
	
	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

list_item_t *treeview_append_row( object_t *treeview, list_item_t *parent, image_t *image, char *title )
{
	assert_valid_treeview_widget( treeview, "treeview" );
	assert_only_list_item( parent, "parent" );
	assert_only_image( image, "image" );
	
	return list_widget_row_append( treeview, parent, image, title );
}

list_item_t *treeview_insert_row( object_t *treeview, list_item_t *parent, int pos, image_t *image, char *title )
{
	assert_valid_treeview_widget( treeview, "treeview" );
	assert_only_list_item( parent, "parent" );
	assert_only_image( image, "image" );
	
	return list_widget_row_insert( treeview, parent, pos, image, title );
}

event_handler( treeview_edit_row_handle )
{
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	cgraphics_treeview_edit_row( WIDGET(object), item );
}

void treeview_new_row_handle( object_t *obj, event_t *event )
{
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	cgraphics_treeview_new_row( WIDGET(obj), item );
}

void treeview_move_row( object_t *treeview, list_item_t *item, int row )
{
	assert_valid_treeview_widget( treeview, "treeview" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_move( treeview, item, row );
}

void treeview_remove_row( object_t *treeview, list_item_t *item )
{
	assert_valid_treeview_widget( treeview, "treeview" );
	assert_valid_list_item( item, "item" );
	
	list_widget_row_remove( treeview, item );
}

void treeview_remove_row_handle( object_t *obj, event_t *event )
{
	treeview_widget_t *lw = (treeview_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	if ( lw->selected == item )
	{
		lw->selected = NULL;
		event_send( OBJECT(lw), "selected", "p", "row", NULL );
	}
	
	cgraphics_treeview_remove_row( WIDGET(obj), item );
}

list_item_t *treeview_get_selected( object_t *obj )
{
	treeview_widget_t *lw = (treeview_widget_t *)obj;
	
	assert_valid_treeview_widget( obj, "obj" );
	
	return lw->selected;
}

int treeview_get_rows( object_t *obj, list_item_t *parent )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	assert_valid_treeview_widget( obj, "obj" );
	assert_only_list_item( parent, "parent" );
	
	if ( parent == 0 )
		return LIST_LENGTH( &lw->items );
	
	return LIST_LENGTH( &parent->children );
}

void treeview_select_item( object_t *obj, list_item_t *item )
{
	assert_valid_treeview_widget( obj, "obj" );
	
	cgraphics_treeview_select_row( WIDGET(obj), item );
}

void treeview_expand( object_t *treeview, list_item_t *item )
{
	assert_valid_treeview_widget( treeview, "treeview" );
	assert_valid_list_item( item, "item" );
	
	cgraphics_treeview_expand( WIDGET(treeview), item );
}

void treeview_collapse( object_t *treeview, list_item_t *item )
{
	assert_valid_treeview_widget( treeview, "treeview" );
	assert_valid_list_item( item, "item" );
	
	cgraphics_treeview_collapse( WIDGET(treeview), item );
}
