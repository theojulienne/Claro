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


#include <claro/base.h>

#include "foo.h"

void foo_inst_create( object_t *object );
void foo_inst_realize( object_t *object );
void foo_inst_finalize( object_t *object );
void foo_inst_destroy( object_t *object );

claro_define_type( foo, object );

void foo_inst_create( object_t *object )
{
	printf( "foo_inst_create(%p)\n", object );
}

void foo_inst_realize( object_t *object )
{
	printf( "foo_inst_realize(%p)\n", object );
}

void foo_inst_finalize( object_t *object )
{
	printf( "foo_inst_finalize(%p)\n", object );
}

void foo_inst_destroy( object_t *object )
{
	printf( "foo_inst_destroy(%p)\n", object );
}

object_t *foo_create( object_t *parent )
{
	object_t *object;
	
	object = object_create_from_class( &foo_class_info, parent );
	
	/* realize the object */
	object_realize( object );
	
	return object;
}

