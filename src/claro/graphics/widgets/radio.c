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

void radiogroup_inst_create( object_t *object );
void radiogroup_inst_realize( object_t *object );

claro_define_type_partial( radiogroup, object, radiogroup_inst_create, radiogroup_inst_realize, NULL, NULL );

void radiogroup_inst_create( object_t *object )
{
	WIDGET(object)->ndata = NULL;
	list_create( &((radiogroup_t *)object)->buttons );
}

void radiogroup_inst_realize( object_t *object )
{
	cgraphics_radiogroup_create( WIDGET(object) );
}

object_t *radiogroup_create( object_t *parent, int flags )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( radiogroup_type, parent );

	object_realize( object );

	return object;
}


void radiobutton_widget_inst_realize( object_t *object );

claro_define_widget_partial( radiobutton, NULL, radiobutton_widget_inst_realize, NULL, NULL );

void radiobutton_widget_inst_realize( object_t *object )
{
	cgraphics_radiobutton_widget_create( WIDGET(object) );
}

object_t *radiobutton_widget_create( object_t *parent, object_t *group, bounds_t *bounds, const char *label, int flags )
{
	object_t *object;
	
	assert_valid_widget( parent, "parent" );
	assert_valid_radiogroup_widget( group, "group" );

	object = object_create_from_class( radiobutton_widget_type, parent );
	
	widget_set_bounds( object, bounds );
	widget_set_flags( object, flags );
	
	object_realize( object );
	
	radiobutton_set_label( object, label );
	radiobutton_set_group( object, group );

	return object;
}

void radiobutton_set_label( object_t *obj, const char *label )
{
	radiobutton_widget_t *chk = (radiobutton_widget_t *)obj;
	
	assert_valid_radiobutton_widget( obj, "obj" );
	
	strscpy( chk->text, label, RADIOBUTTON_TEXT_MAX );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( obj ) )
		return;
	
	cgraphics_radiobutton_set_text( WIDGET(obj) );
}

void radiobutton_set_group( object_t *radio, object_t *group )
{
	radiobutton_widget_t *r = (radiobutton_widget_t *)radio;
	radiogroup_t *g = (radiogroup_t *)group;
	node_t *n;
	
	assert_valid_radiobutton_widget( radio, "radio" );
	assert_valid_radiogroup_widget( group, "group" );
	
	if ( r->group != 0 )
	{
		/* FIXME: remove from old group ! */
	}
	
	r->group = g;
	
	n = node_create( );
	
	node_add( radio, n, &g->buttons );
	
	/* skip if object not yet realized */
	if ( !object_is_realized( radio ) )
		return;
	
	cgraphics_radiobutton_set_group( WIDGET(radio) );
}
