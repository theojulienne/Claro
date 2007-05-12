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

void window_widget_inst_realize( object_t *object );

claro_define_widget_partial( window, NULL, window_widget_inst_realize, NULL, NULL );

void window_widget_inst_realize( object_t *object )
{
	cgraphics_window_widget_create( WIDGET(object) );
}

object_t *window_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;

	object = object_create_from_class( window_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );

	object_realize( object );
	
	widget_set_position( object, bounds->x, bounds->y, 0 );
	widget_set_content_size( object, bounds->w, bounds->h, 0 );
	
	object_addhandler( object, "content_resized", widget_resized_handle );

	return object;
}

void window_show( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_show( WIDGET(w) );
}

void window_hide( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_hide( WIDGET(w) );
}

void window_focus( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_focus( WIDGET(w) );
}

void window_set_title( object_t *w, const char *title )
{
	window_widget_t *ww = (window_widget_t *)w;
	
	assert_valid_window_widget( w, "w" );
	
	strncpy( ww->title, title, 512 );
	
	cgraphics_window_update_title( WIDGET(w) );
}

void window_maximise( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_maximise( WIDGET(w) );
}

void window_minimise( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_minimise( WIDGET(w) );
}

void window_restore( object_t *w )
{
	assert_valid_window_widget( w, "w" );
	cgraphics_window_restore( WIDGET(w) );
}

void window_set_icon( object_t *w, image_t *icon )
{
	window_widget_t *ww = (window_widget_t *)w;
	
	assert_valid_window_widget( w, "w" );
	assert_valid_image( icon, "icon" );
	
	ww->icon = icon;
	cgraphics_window_update_icon( WIDGET(w) );
}
