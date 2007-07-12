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

void object_inst_create( object_t *object );
void object_inst_realize( object_t *object );
void object_inst_finalize( object_t *object );
void object_inst_destroy( object_t *object );

/* object must be defined manually, it has no parent */
class_info_t object_class_info =
{
	"object",
	sizeof( object_t ),

	NULL, /* parent class */

	object_inst_create, /* create func */
	object_inst_realize, /* realize func */

	object_inst_finalize, /* finalize func */
	object_inst_destroy, /* destroy func */
};

class_type_t *object_get_type( )
{
	static class_type_t *type = NULL;
	if ( type == NULL ) {
		type = get_type_for_class( &object_class_info );
	}
	return type;
}


/* global object and destruction lists */
//list_t object_list = {};
//list_t destroy_list = {};


/* class instance functions for object */

void object_inst_create( object_t *object )
{
    object->event_handlers = claro_list_create();
    object->children = claro_list_create();
    object->appdata = NULL;
}

void object_inst_realize( object_t *object )
{
	
}

void object_inst_finalize( object_t *object )
{
	event_send( object, "finalized", "" );
}

//this is all REALLY goofy - going to go away with ref-counting

void object_inst_destroy( object_t *object )
{
/*	node_t *n, *nn, *tn;
	
	if ( !( n = node_find( object, &object_list ) ) )
		return;
*/
    int i, len;
	
    g_return_if_fail(object != NULL);

	/* clean up parenting */
	object_set_parent( object, NULL );

// iterate children- this should decrement the reference along with setting 'parent' to NULL	
    len = claro_list_count(object->children);
    
    for(i = 0; i < len; i++)
    {
        object_set_parent(claro_list_get_item(object->children, i), NULL);
    }

    claro_list_destroy(object->children);

// iterate the event handlers- this should be a hashtable of event names to a list

    len = claro_list_count(object->event_handlers);

    for(i = 0; i < len; i++)
    {
        sfree(claro_list_get_item(object->event_handlers, i));
    }    
    
    claro_list_destroy(object->event_handlers);

	/* make all our children have no parent.
	   FIXME: there should be a flag to destroy children
	   NOTE: widget_destroy will automatically handle children through the OS, though? */
/*
#ifndef OLD_CHILDREN
	for ( ; object->children->len > 0;  )
	{
		object_set_parent( g_ptr_array_index( object->children, 0 ), NULL );
	}
#else
	LIST_FOREACH_SAFE( nn, tn, object->children.head )
	{
		// remove the parenting; this will free the current node too 
		object_set_parent( (object_t *)nn->data, NULL );
	}
#endif
	
	// remove all handlers 
	LIST_FOREACH_SAFE( nn, tn, object->event_handlers.head )
	{
		free( nn->data );
		node_del( nn, &object->event_handlers );
		node_free( nn );
	}
	
	// remove the object from the global list, kill the node 
	node_del( n, &object_list );
	node_free( n );
*/	
	/* clean up the memory */

	free( object );
}

int object_is_realized( object_t *object ) 
{
    g_return_val_if_fail(object != NULL, 0);
	return object->realized;
}


// this is now a bit pointless

void object_queue_destruction( object_t *object )
{
    g_return_if_fail(object != NULL);
/*
	node_t *n;
	
	n = node_create( );
	node_add( object, n, &destroy_list );
*/
}

void object_set_parent( object_t *object, object_t *parent )
{
    g_return_if_fail(object != NULL);

    if ( object->parent != NULL )
        g_assert(claro_list_remove(object->parent->children, object) == TRUE);

    if(parent != NULL)
        claro_list_append(parent->children, object);
        
    object->parent = parent;

    if(parent != NULL)
        event_send( object, "parent_attach", "" );

/*
#ifdef OLD_CHILDREN
	node_t *n;
#endif
	
	if ( obj->parent != NULL )
	{
#ifndef OLD_CHILDREN
		if ( g_ptr_array_remove( obj->parent->children, obj ) == false )
		{
			clog( CL_ERROR, "Object has a parent listed, but the parent's children list doesn't contain it." );
		}
#else
		// remove old parenting 
		n = node_find( obj, &obj->parent->children );
		
		if ( n != NULL )
		{
			node_del( n, &obj->parent->children );
			node_free( n );
		}
		else
			clog( CL_ERROR, "Object has a parent listed, but the parent's children list doesn't contain it." );
#endif
	}
	
	if ( parent != NULL )
	{
		// add obj to parent's children list 
#ifndef OLD_CHILDREN
		g_ptr_array_add( parent->children, obj );
#else
		n = node_create( );
		node_add( obj, n, &parent->children );
#endif
	}
	
	obj->parent = parent;
	
	if ( parent != NULL )
		event_send( obj, "parent_attach", "" );
*/
}


