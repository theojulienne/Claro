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

void checkbox_widget_inst_realize( object_t *object );

claro_define_widget_partial( checkbox, NULL, checkbox_widget_inst_realize, NULL, NULL );

void checkbox_widget_inst_realize( object_t *object )
{
	cgraphics_checkbox_widget_create( WIDGET(object) );
}

object_t *checkbox_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, const char *label )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( checkbox_widget_type, parent );

	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	checkbox_set_label( object, label );

	object_realize( object );

	return object;
}

object_t *checkbox_widget_create( object_t *parent, bounds_t *bounds, int flags )
{
	return checkbox_widget_create_with_label( parent, bounds, flags, "" );
}

void checkbox_set_label( object_t *obj, const char *label )
{
	checkbox_widget_t *chk = (checkbox_widget_t *)obj;
	
	assert_valid_checkbox_widget( obj, "obj" );
	
	strscpy( chk->text, label, CHECKBOX_TEXT_MAX );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( obj ) )
		return;
	
	cgraphics_checkbox_set_text(WIDGET(obj));
}

int checkbox_get_checked( object_t *obj )
{
	checkbox_widget_t *chk = (checkbox_widget_t *)obj;
	
	assert_valid_checkbox_widget( obj, "obj" );
	
	return chk->checked;
}

void checkbox_set_checked( object_t *obj, int checked )
{
	checkbox_widget_t *chk = (checkbox_widget_t *)obj;
	
	assert_valid_checkbox_widget( obj, "obj" );
	
	chk->checked = checked;
	
	/* skip if object not yet realized */
	if ( !object_is_realized( obj ) )
		return;
	
	cgraphics_checkbox_update_checked( WIDGET(obj) );
}
