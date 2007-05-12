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


void button_widget_inst_realize( object_t *object );

claro_define_widget_partial( button, NULL, button_widget_inst_realize, NULL, NULL );

void button_widget_inst_realize( object_t *object )
{
	cgraphics_button_widget_create( WIDGET(object) );
}

object_t *button_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( button_widget_type, parent );
	
	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	object_realize( object );

	return object;
}

object_t *button_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label )
{
	object_t *o;
	
	assert_valid_widget( parent, "parent" );
	
	o = button_widget_create( parent, bounds, flags );
	button_set_text( o, label );
	
	return o;
}

void button_set_label( object_t *obj, const char *label )
{
	button_widget_t *bw = (button_widget_t *)obj;
	
	assert_valid_button_widget( obj, "obj" );
	
	strscpy( bw->text, label, CLARO_BUTTON_MAXIMUM );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( obj ) )
		return;
	
	cgraphics_button_update_text( bw );
}

void button_set_image( object_t *obj, image_t *image )
{
	assert_valid_button_widget( obj, "obj" );
	assert_valid_image( image, "image" );
	
	/*cgraphics_button_update_image((button_widget_t *)obj, filename);*/
}
