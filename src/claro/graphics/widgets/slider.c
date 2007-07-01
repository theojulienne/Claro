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

void slider_widget_inst_realize( object_t *object );

claro_define_widget_partial( slider, NULL, slider_widget_inst_realize, NULL, NULL );

void slider_widget_inst_realize( object_t *object )
{
	cgraphics_slider_widget_create( WIDGET(object) );
}

object_t *slider_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( slider_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );

	object_realize( object );
	
	slider_set_range( object, 0, 1 );

	return object;
}

void slider_set_range( object_t *slider, double minimum, double maximum )
{
	assert_valid_slider_widget( slider, "slider" );
	
	cgraphics_slider_set_range( WIDGET(slider), minimum, maximum );
}

void slider_set_value( object_t *slider, double value )
{
	assert_valid_slider_widget( slider, "slider" );
	
	cgraphics_slider_set_value( WIDGET(slider), value );
}

double slider_get_value( object_t *slider )
{
	assert_valid_slider_widget( slider, "slider" );
	
	return cgraphics_slider_get_value( WIDGET(slider) );
}

void slider_set_style( object_t *slider, int style )
{
	assert_valid_slider_widget( slider, "slider" );
	
	cgraphics_slider_set_style( WIDGET(slider), style );
}

