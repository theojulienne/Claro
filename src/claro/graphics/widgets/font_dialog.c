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

void font_dialog_widget_inst_realize( object_t *object );

claro_define_widget_partial( font_dialog, NULL, font_dialog_widget_inst_realize, NULL, NULL );

void font_dialog_widget_inst_realize( object_t *object )
{
	cgraphics_font_dialog_widget_create( WIDGET(object) );
}

object_t *font_dialog_widget_create( object_t *parent, int flags )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( font_dialog_widget_type, parent );

	//widget_set_bounds( object, NO_BOUNDS );
	widget_set_flags( object, flags );

	object_realize( object );

	return object;
}

void font_dialog_set_font( object_t *obj, const char *face, int size, int weight, int slant, int decoration )
{
	font_dialog_widget_t *fdw = (font_dialog_widget_t *)obj;
	
	assert_valid_font_dialog_widget( obj, "obj" );
	
	if ( fdw->selected.face != NULL )
		free( fdw->selected.face );
	
	fdw->selected.face = strdup( face );
	fdw->selected.size = size;
	fdw->selected.weight = weight;
	fdw->selected.slant = slant;
	fdw->selected.decoration = decoration;
	
	cgraphics_font_dialog_update_from_selection( WIDGET(obj) );
}

font_t *font_dialog_get_font( object_t *obj )
{
	font_dialog_widget_t *fdw = (font_dialog_widget_t *)obj;
	
	assert_valid_font_dialog_widget( obj, "obj" );
	
	cgraphics_font_dialog_update_to_selection( WIDGET(obj) );
	
	return &fdw->selected;
}
