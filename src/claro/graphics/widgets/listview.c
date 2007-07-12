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

void listview_new_row_handle( object_t *obj, event_t *event );
void listview_remove_row_handle( object_t *obj, event_t *event );

void listview_widget_inst_realize( object_t *object );

claro_define_type_partial( listview_widget, list_widget, NULL, listview_widget_inst_realize, NULL, NULL );

void listview_widget_inst_realize( object_t *object )
{
	cgraphics_listview_widget_create_columns( WIDGET(object) );
}

void listview_handle_destroy( listview_widget_t *obj, event_t *e );

object_t *listview_widget_create( object_t *parent, bounds_t *bounds, int columns, int flags, ... )
{
	va_list argp;
	int c;
	int cols[columns];
	listview_widget_t *lw;
	object_t *obj;
	
	va_start( argp, flags );
	
	assert_valid_widget( parent, "parent" );
	
	obj = object_create_from_class( listview_widget_type, parent );
	
	widget_set_bounds( obj, bounds );
	widget_set_flags( obj, flags );
	
	lw = (listview_widget_t *)obj;
	
	lw->titles = (char **)malloc( sizeof(char *) * columns );
	
	for ( c = 0; c < columns; c++ )
	{
		char *tmp = va_arg( argp, char * );
		int type = va_arg( argp,  int );
		
		lw->titles[c] = strdup( tmp );
		
		switch ( type )
		{
			case CLISTVIEW_TYPE_CHECKBOX:
				cols[c] = CLIST_TYPE_UINT;
				break;
			case CLISTVIEW_TYPE_PROGRESS:
				cols[c] = CLIST_TYPE_DOUBLE;
				break;
			default:
			case CLISTVIEW_TYPE_TEXT:
				cols[c] = CLIST_TYPE_STRING;
				break;
		}
	}
	
	//list_widget_init( obj, 1, CLIST_TYPE_STRING );
	list_widget_init_ptr( obj, columns, &cols );
	
	/* handle list operations */
	object_addhandler( obj, "new_row", listview_new_row_handle );
	object_addhandler( obj, "remove_row", listview_remove_row_handle );
	object_addhandler( obj, "destroy", listview_handle_destroy );
	
	va_end( argp );
	
	object_realize( obj );
	
	return obj;
}

CLFEXP object_t *listview_widget_create_with_columns( object_t *parent, bounds_t *bounds, int num_columns, int flags, claro_listview_column_t * columns )
{
	int c;
	int cols[num_columns];
	listview_widget_t *lw;
	object_t *obj;
	
	assert_valid_widget( parent, "parent" );
	
	obj = object_create_from_class( listview_widget_type, parent );
	
	widget_set_bounds( obj, bounds );
	widget_set_flags( obj, flags );
	
	lw = (listview_widget_t *)obj;
	
	lw->titles = (char **)malloc( sizeof(char *) * num_columns );
	
	for ( c = 0; c < num_columns; c++ )
	{
		char *tmp = columns[c].name;
		int type = columns[c].type;
		
		lw->titles[c] = sstrdup( tmp );
		
		switch ( type )
		{
			case CLISTVIEW_TYPE_CHECKBOX:
				cols[c] = CLIST_TYPE_UINT;
				break;
			case CLISTVIEW_TYPE_PROGRESS:
				cols[c] = CLIST_TYPE_DOUBLE;
				break;
			default:
			case CLISTVIEW_TYPE_TEXT:
				cols[c] = CLIST_TYPE_STRING;
				break;
		}
	}
	
	//list_widget_init( obj, 1, CLIST_TYPE_STRING );
	list_widget_init_ptr( obj, columns, &cols );
	
	/* handle list operations */
	object_addhandler( obj, "new_row", listview_new_row_handle );
	object_addhandler( obj, "remove_row", listview_remove_row_handle );
	object_addhandler( obj, "destroy", listview_handle_destroy );
	
	object_realize( obj );
	
	return obj;  
}


void listview_handle_destroy( listview_widget_t *obj, event_t *e )
{
	int a;
	
	for ( a = 0; a < obj->widget.columns; a++ )
	{
		free( obj->titles[a] );
	}
	
	free( obj->titles );
}

list_item_t *listview_append_row( object_t *listview, ... )
{
	list_item_t *item;
	va_list argp;
	
	assert_valid_listview_widget( listview, "listview" );
	
	va_start( argp, listview );
	
	item = list_widget_row_insert_ptr( listview, 0, -1, argp );
	
	va_end( argp );
	
	return item;
}

list_item_t *listview_insert_row( object_t *listview, int pos, ... )
{
	list_item_t *item;
	va_list argp;
	
	assert_valid_listview_widget( listview, "listview" );
	
	va_start( argp, listview );
	
	item = list_widget_row_insert_ptr( listview, 0, pos, argp );
	
	va_end( argp );
	
	return item;
}

void listview_new_row_handle( object_t *obj, event_t *event )
{
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	cgraphics_listview_new_row( WIDGET(obj), item );
}

void listview_move_row( object_t *listview, list_item_t *item, int row )
{
	assert_valid_listview_widget( listview, "listview" );
	
	list_widget_row_move( listview, item, row );
}

void listview_remove_row( object_t *listview, list_item_t *item )
{
	assert_valid_listview_widget( listview, "listview" );
	
	list_widget_row_remove( listview, item );
}

void listview_remove_row_handle( object_t *obj, event_t *event )
{
	listview_widget_t *lw = (listview_widget_t *)obj;
	list_item_t *item = (list_item_t *)event_get_ptr( event, "row" );
	
	if ( lw->selected == item )
	{
		lw->selected = NULL;
		event_send( OBJECT(lw), "selected", "p", "row", NULL );
	}
	
	cgraphics_listview_remove_row( WIDGET(obj), item );
}

list_item_t *listview_get_selected( object_t *obj )
{
	listview_widget_t *lw = (listview_widget_t *)obj;
	
	assert_valid_listview_widget( obj, "obj" );
	
	return lw->selected;
}

int listview_get_rows( object_t *obj )
{
	list_widget_t *lw = (list_widget_t *)obj;
	
	assert_valid_listview_widget( obj, "obj" );
	
	return LIST_LENGTH( &lw->items );
}

void listview_select_item( object_t *obj, list_item_t *item )
{
	assert_valid_listview_widget( obj, "obj" );
	cgraphics_listview_select_row( WIDGET(obj), item );
}
