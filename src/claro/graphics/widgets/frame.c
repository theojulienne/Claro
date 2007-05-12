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

void frame_widget_inst_realize( object_t *object );

claro_define_widget_partial( frame, NULL, frame_widget_inst_realize, NULL, NULL );

void frame_widget_inst_realize( object_t *object )
{
	cgraphics_frame_widget_create( WIDGET(object) );
}

object_t *frame_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( frame_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	frame_set_text( object, label );

	object_realize( object );

	return object;
}

object_t *frame_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	return frame_widget_create_with_label( parent, bounds, flags, "" );
}

void frame_set_text( object_t *frame, const char *label )
{
	frame_widget_t *fw = (frame_widget_t *)frame;
	
	assert_valid_frame_widget( frame, "frame" );
	
	strscpy( fw->text, label, CLARO_FRAME_MAXIMUM );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( frame ) )
		return;
	
	cgraphics_frame_set_text((widget_t *)frame);
}
