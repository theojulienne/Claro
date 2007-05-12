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

void statusbar_widget_inst_realize( object_t *object );

claro_define_widget_partial( statusbar, NULL, statusbar_widget_inst_realize, NULL, NULL );

void statusbar_widget_inst_realize( object_t *object )
{
	cgraphics_statusbar_widget_create( WIDGET(object) );
}

object_t *statusbar_widget_create( object_t *parent, int flags )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( statusbar_widget_type, parent );

	widget_set_flags( object, flags );

	object_realize( object );

	return object;
}

void statusbar_set_text( object_t *obj, const char *text )
{
	statusbar_widget_t *sb = (statusbar_widget_t *)obj;
	
	assert_valid_statusbar_widget( obj, "obj" );
	
	strscpy( sb->text, text, CLARO_BUTTON_MAXIMUM );
	cgraphics_statusbar_widget_update_text(sb);
}
