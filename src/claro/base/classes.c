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



static GHashTable * _type_table;

/* object system initialization */
void object_init( )
{
    _type_table = g_hash_table_new(g_str_hash, g_str_equal);
}

void * claro_ref(void * object)
{
    return object;    
}

void claro_unref(void * object)
{
}

class_info_t * claro_register_class(const class_info_t * info)
{
    class_info_t * found_info = g_hash_table_lookup(_type_table, info->name);    
    g_assert(found_info == NULL);

    g_hash_table_insert(_type_table, info->name, info);

    return info;
}

void object_run_class_create( object_t *object, const class_info_t *class_info )
{
	/* if we are not the root object class, run our parent create */
	if ( class_info->parent_class != NULL )
		object_run_class_create( object, class_info->parent_class );
	
	/* do we have a create function? */
	if ( class_info->create_func != NULL )
		(*class_info->create_func)( object );
}

object_t *object_create_from_class( class_type_t *class_type, object_t *parent )
{
	const class_info_t *class_info = class_type->info;
	object_t *obj = NULL;
	
	obj = (object_t *)smalloc( class_info->object_size );
	
	memset( obj, 0, class_info->object_size );
	
	obj->class_type = class_type;
	obj->realized = 0;
	
	//strcpy( obj->type, class_info->name );
    obj->type = class_info->name;	
    obj->destroy_pending = 0;
	
	/* run create functions */
	object_run_class_create( obj, obj->class_type->info );
	
	/* set our parent */
	object_set_parent( obj, parent );
	
	return obj;
}

void object_run_class_realize( object_t *object, const class_info_t *class_info )
{
	/* if we are not the root object class, run our parent realize */
	if ( class_info->parent_class != NULL )
		object_run_class_realize( object, class_info->parent_class );
	
	/* do we have a realize function? */
	if ( class_info->realize_func != NULL )
		(*class_info->realize_func)( object );
}

void object_realize( object_t *object )
{
	object_run_class_realize( object, object->class_type->info );
	
	object->realized = 1;
	
	event_send( object, "realized", "" );
}

/* used by object_destroy to run class finalization on an instance */
void object_run_class_finalize( object_t *object, const class_info_t *class_info )
{
	/* do we have a finalize function? */
	if ( class_info->finalize_func != NULL )
		(*class_info->finalize_func)( object );
	
	/* if we are not the root object class, run our parent finalize */
	if ( class_info->parent_class != NULL )
		object_run_class_finalize( object, class_info->parent_class );
}

/* object destroy function, finalizes an object and queues it for destruction */
void object_destroy( object_t *object )
{
	if ( object->destroy_pending == 1 )
		return; // already in destroy list.
	
	// save our destroy status
	object->destroy_pending = 1;
	
	// queue object for destruction
	object_queue_destruction( object );
	
	// run finalize functions
	object_run_class_finalize( object, object->class_type->info );
}

/* used by object_actual_destroy to run class destruction on an instance */
void object_run_class_destroy( object_t *object, const class_info_t *class_info )
{
	/* do we have a finalize function? */
	if ( class_info->destroy_func != NULL )
		(*class_info->destroy_func)( object );
	
	/* if we are not the root object class, run our parent finalize */
	if ( class_info->parent_class != NULL )
		object_run_class_destroy( object, class_info->parent_class );
}

/* object real destruction function, calls class destroy functions */
void object_actual_destroy( object_t *object )
{
	if ( object->destroy_pending == 0 )
	{
		/* object must be finalized first. */
		object_destroy( object );
		return;
	}
	
	object_run_class_destroy( object, object->class_type->info );
}

int object_is_of_class( object_t *object, const char *cl_name )
{
	const class_info_t *ci;
	
	for ( ci = object->class_type->info; ci != NULL; ci = ci->parent_class )
	{
		if ( !strcmp( ci->name, cl_name ) )
			return true;
	}
	
	return false;
}

class_type_t *get_type_for_class( const class_info_t *info )
{
	class_type_t *type;
	
	// FIXME: block allocate this
	type = (class_type_t *)smalloc( sizeof(class_type_t) );
	
	// FIXME: store this for class analysis
	
	type->info = info;
	
	return type;
}

#if 0
const class_info_t widget_class_info =
{
	"widget",
	sizeof( object_t ),
	
	&object_class_info,
	
	NULL, /* create func */
	NULL, /* realize func */
	
	NULL, /* finalize func */
	NULL, /* destroy func */
};

void button_widget_inst_create( object_t *object ) {
	printf( "button_widget_inst_create()\n" );
}

const class_info_t button_widget_class_info =
{
	"button_widget",
	sizeof( object_t ),
	
	&widget_class_info,
	
	button_widget_inst_create, /* create func */
	NULL, /* realize func */
	
	NULL, /* finalize func */
	NULL, /* destroy func */
};

object_t *buttonx_widget_create( object_t *parent, void *bounds, int flags )
{
	object_t *obj;
	
	//assert_valid_widget( parent, "parent" );
	
	/* create the object from our class info structure,
	 * this will run initialization functions but not realization funcs.
	 */
	obj = object_create_from_class( &button_widget_class_info, parent );
	
	/* set some values based on our parameters, these are required for realization */
	//widget_set_bounds( obj, bounds );
	//widget_set_flags( obj, flags );
	
	/* realize the object */
	object_realize( obj );
	
	return obj;
}
#endif
