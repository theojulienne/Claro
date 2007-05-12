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

void label_widget_inst_realize( object_t *object );

claro_define_widget_partial( label, NULL, label_widget_inst_realize, NULL, NULL );

void label_widget_inst_realize( object_t *object )
{
	cgraphics_label_widget_create( WIDGET(object) );
}

object_t *label_widget_create_with_text( object_t *parent, bounds_t *bounds, int flags, const char *text )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( label_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );

	label_set_text( object, text );
	label_set_justify( object, cLabelLeft );

	object_realize( object );

	return object;
}

object_t *label_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	return label_widget_create_with_text( parent, bounds, flags, "" );
}

void label_set_justify( object_t *obj, int flags )
{
	WIDGET(obj)->flags = flags;
	
	assert_valid_label_widget( obj, "obj" );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( obj ) )
		return;
	
	cgraphics_label_update_justify( (widget_t *)obj );
}

void label_set_text( object_t *obj, const char *text )
{
	label_widget_t *w = (label_widget_t *)obj;
	
	assert_valid_label_widget( obj, "obj" );
	
	strscpy( w->text, text, CLARO_LABEL_MAXIMUM );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( obj ) )
		return;
	
	cgraphics_label_update_text( (widget_t *)w );
}
