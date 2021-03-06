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

#ifdef CLARO_OPENGL

void opengl_widget_inst_realize( object_t *object );

claro_define_widget_partial( opengl, NULL, opengl_widget_inst_realize, NULL, NULL );

void opengl_widget_inst_realize( object_t *object )
{
	cgraphics_opengl_widget_create( WIDGET(object) );
}

object_t *opengl_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( opengl_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );

	object_realize( object );

	return object;
}

void opengl_flip( object_t *widget )
{
	assert_valid_opengl_widget( widget, "widget" );
	cgraphics_opengl_flip( WIDGET(widget) );
}

void opengl_activate( object_t *widget )
{
	assert_valid_opengl_widget( widget, "widget" );
	cgraphics_opengl_activate( WIDGET(widget) );
}

#endif