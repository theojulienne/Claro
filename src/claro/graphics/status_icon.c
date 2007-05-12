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


void status_icon_inst_realize( object_t *object );

claro_define_type_partial( status_icon, object, NULL, status_icon_inst_realize, NULL, NULL );

void status_icon_inst_realize( object_t *object )
{
	cgraphics_status_icon_create( WIDGET(object) );
}

object_t *status_icon_create( object_t *parent, image_t *icon, int flags )
{
	object_t *object;

	assert_valid_widget( parent, "parent" );

	object = object_create_from_class( status_icon_type, parent );

	cgraphics_status_icon_create( object, flags );
    cgraphics_status_icon_set_icon( object, icon );

	object_realize( object );

	return object;
}

void status_icon_set_icon(object_t * status, image_t * icon)
{
    assert_valid_status_icon(status, "status");
    
    cgraphics_status_icon_set_icon((status_icon_t*)status, icon);
}

void status_icon_set_menu(object_t * status, object_t * menu)
{
    assert_valid_status_icon(status, "status");
    assert_valid_menu_widget(menu, "menu");
    
    cgraphics_status_icon_set_menu((status_icon_t*)status, menu);
}

void status_icon_set_visible(object_t * status, int visible)
{
    assert_valid_status_icon(status, "status");
    
    cgraphics_status_icon_set_visible((status_icon_t*)status, visible);
}

void status_icon_set_tooltip(object_t * status, const char * tooltip)
{
    assert_valid_status_icon(status, "status");
    
    cgraphics_status_icon_set_tooltip((status_icon_t*)status, tooltip);
}



