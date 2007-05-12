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

claro_define_type_partial( image, object, NULL, NULL, NULL, NULL );

image_t *image_load( object_t *parent, const char *file )
{
	object_t *object;

	object = object_create_from_class( image_type, parent );
	
	cgraphics_image_load( (image_t *)object, file );

	object_realize( object );

	return (image_t *)object;
}

image_t *image_load_inline_png( object_t *parent, const unsigned char * data, int len)
{
	object_t *object;

	object = object_create_from_class( image_type, parent );
	
	cgraphics_image_load_inline_png( (image_t *)object, data, len );

	object_realize( object );

	return (image_t *)object;
}
