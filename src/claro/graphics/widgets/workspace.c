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

void workspace_new_row_handle( object_t *obj, event_t *event );
void workspace_remove_row_handle( object_t *obj, event_t *event );

void workspace_widget_inst_realize( object_t *object );

claro_define_widget_partial( workspace, NULL, workspace_widget_inst_realize, NULL, NULL );

void workspace_widget_inst_realize( object_t *object )
{
	window_widget_t *pwin;
	
	cgraphics_workspace_widget_create( WIDGET(object) );
	
	pwin = (window_widget_t *)widget_get_window( object );
	pwin->workspace = WIDGET(object);
}

object_t *workspace_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( workspace_widget_type, parent );
	
	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

void workspace_set_active( object_t *workspace, object_t *child )
{
	assert_valid_workspace_widget( workspace, "workspace" );
	assert_valid_workspace_window_widget( child, "child" );
	
	cgraphics_workspace_set_active( WIDGET(workspace), child );
}

object_t *workspace_get_active( object_t *workspace )
{
	assert_valid_workspace_widget( workspace, "workspace" );
	
	return cgraphics_workspace_get_active( WIDGET(workspace) );
}

void workspace_cascade( object_t *workspace )
{
	assert_valid_workspace_widget( workspace, "workspace" );
	
	cgraphics_workspace_cascade( WIDGET(workspace) );
}

void workspace_tile( object_t *workspace, int dir )
{
	assert_valid_workspace_widget( workspace, "workspace" );
	
	cgraphics_workspace_tile( WIDGET(workspace), dir );
}

/** workspace window **/

void workspace_window_widget_inst_realize( object_t *object );

const class_info_t workspace_window_widget_class_info =
{
	"workspace_window_widget",
	sizeof( workspace_window_widget_t ),

	&widget_class_info, /* parent class */

	NULL, /* create func */
	workspace_window_widget_inst_realize, /* realize func */

	NULL, /* finalize func */
	NULL, /* destroy func */
};

void workspace_window_widget_inst_realize( object_t *object )
{
	cgraphics_workspace_window_widget_create( WIDGET(object) );
}

object_t *workspace_window_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( &workspace_window_widget_class_info, parent );
	
	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

void workspace_window_set_title( object_t *window, char *title )
{
	workspace_window_widget_t *ww = (workspace_window_widget_t *)window;
	
	assert_valid_workspace_window_widget( window, "window" );
	
	strncpy( ww->title, title, 512 );
	
	cgraphics_workspace_window_update_title( WIDGET(window) );
}

void workspace_window_show( object_t *window )
{
	assert_valid_workspace_window_widget( window, "window" );
	cgraphics_workspace_window_show( WIDGET(window) );
}

void workspace_window_hide( object_t *window )
{
	assert_valid_workspace_window_widget( window, "window" );
	cgraphics_workspace_window_hide( WIDGET(window) );
}

void workspace_window_restore( object_t *window )
{
	assert_valid_workspace_window_widget( window, "window" );
	cgraphics_workspace_window_restore( WIDGET(window) );
}

void workspace_window_minimise( object_t *window )
{
	assert_valid_workspace_window_widget( window, "window" );
	cgraphics_workspace_window_minimise( WIDGET(window) );
}

void workspace_window_maximise( object_t *window )
{
	assert_valid_workspace_window_widget( window, "window" );
	cgraphics_workspace_window_maximise( WIDGET(window) );
}

void workspace_window_set_icon( object_t *w, image_t *icon )
{
	workspace_window_widget_t *ww = (workspace_window_widget_t *)w;
	
	assert_valid_workspace_window_widget( w, "w" );
	assert_valid_image( icon, "icon" );
	
	ww->icon = icon;
	cgraphics_workspace_window_update_icon( WIDGET(w) );
}
