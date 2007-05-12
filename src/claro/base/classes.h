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

#ifndef _CLARO_BASE_CLASSES_H
#define _CLARO_BASE_CLASSES_H

/* create: when an object has just been allocated */
typedef void object_create_func( object_t *object );

/* realize: when an object is to be made real */
typedef void object_realize_func( object_t *object );

/* finalize: opposite of realize, clear up native code, etc */
typedef void object_finalize_func( object_t *object );

/* destroy: free any memory allocated in init */
typedef void object_destroy_func( object_t *object );

typedef struct class_info_ class_info_t;

typedef struct class_type_ class_type_t;

struct class_type_
{
	const class_info_t *info;
};

struct class_info_
{
	char name[32];

	int object_size;
	
	const class_info_t *parent_class;
	
	object_create_func *create_func;
	object_realize_func *realize_func;
	
	object_finalize_func *finalize_func;
	object_destroy_func *destroy_func;
};

/* class-related object functions */
CLFEXP object_t *object_create_from_class( class_type_t *class_type, object_t *parent );
CLFEXP void object_realize( object_t *object );
CLFEXP void object_destroy( object_t *object );
CLFEXP void object_actual_destroy( object_t *object );

CLFEXP class_type_t *get_type_for_class( const class_info_t *info );

/* handy macro to declare class_info for a type */

#define claro_define_type_partial(type_name, parent, create_func, realize_func, finalize_func, destroy_func) \
const class_info_t type_name##_class_info = \
{ \
	#type_name, \
	sizeof( type_name##_t ), \
 \
	& parent##_class_info, \
 \
	create_func, \
	realize_func, \
 \
	finalize_func, \
	destroy_func, \
}; \
class_type_t * type_name##_get_type( ) \
{ \
	static class_type_t *type = NULL; \
	if ( type == NULL ) { type = get_type_for_class( &type_name##_class_info ); } \
	return type; \
}

#define claro_define_type(type_name, parent) \
claro_define_type_partial(type_name, parent, type_name ## _inst_create, type_name ## _inst_realize, \
							type_name ## _inst_finalize, type_name ## _inst_destroy)

#define declare_class(type_name) extern const struct class_info_ type_name ## _class_info; \
							class_type_t * type_name ## _get_type( );

int object_is_of_class( object_t *object, const char *cl_name );

#endif
