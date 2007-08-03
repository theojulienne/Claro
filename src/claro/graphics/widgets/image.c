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

void image_widget_inst_realize( object_t *object );

claro_define_widget_partial( image, NULL, image_widget_inst_realize, NULL, NULL );

void image_widget_inst_realize( object_t *object )
{
	cgraphics_image_widget_create( WIDGET(object) );
}

object_t *image_widget_create_with_image( object_t *parent, bounds_t *bounds, int flags, image_t *image )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( image_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );

	object_realize( object );
	
	if ( image != NULL )
		image_set_image( object, image );

	return object;
}

object_t *image_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	return image_widget_create_with_image( parent, bounds, flags, NULL );
}

void image_set_image( object_t *image, image_t *src )
{
	image_widget_t *iw = (image_widget_t *)image;
	
	assert_valid_image_widget( image, "image" );
	assert_valid_image( src, "src" );
	
	iw->src = src;
	
	/* skip if object not yet realized */
	if ( !object_is_realized( image ) )
		return;
	
	cgraphics_image_set_image( WIDGET(image) );
}
