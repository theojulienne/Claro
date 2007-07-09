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

/* perhaps add free value function pointer here */
/*
typedef struct 
{
    char type;
    gpointer val;
} event_arg_t;

static void _free_event_arg(gpointer arg)
{
    event_arg_t * p = (event_arg_t*)arg;
    clog(CL_DEBUG, "%s: %c at %p\n", __FUNCTION__, p->type, p->val); 
    if(arg)
        free(arg);
}
*/

static void _free_event_arg(gpointer arg)
{
	claro_typebox_destroy( (claro_typebox_t *)arg );
}

int event_send( object_t *object, const char *event, const char *fmt, ... )
{
	va_list argp;
//	node_t *n;
	event_handler_t *h;
	event_t e;
	int hn = 0;
	char tmp[1024];
	int a, i, len;
    bool_t mainloop;

	strcpy( e.name, event );
	e.object = object;
	e.arg_num = strlen( fmt );
	strncpy( e.format, fmt, 16 );
	
    mainloop = strcmp( event, "mainloop" ) == 0 ? TRUE : FALSE;

    e.args = NULL;

    if(mainloop)
        goto call_handlers;
    
    e.args = claro_hashtable_str_create(TRUE, _free_event_arg);
	
    va_start( argp, fmt );

	for ( a = 0; a < e.arg_num; a++ )
	{
		char *name = va_arg( argp, char * );
		claro_typebox_t *value = NULL;		

		switch ( fmt[a] )
		{
			case 'p':
				value = claro_typebox_create( fmt[a], va_arg( argp, void * ) );
                break;
			case 'i':
				value = claro_typebox_create( fmt[a], va_arg( argp, int ) );
				break;
			case 'd':
				value = claro_typebox_create( fmt[a], va_arg( argp, double ) );
				break;
			default:
				value = NULL; /* this is bad. */
		}
		
		if ( value == NULL )
			continue;

        claro_hashtable_insert(e.args, (void*)sstrdup(name), (void*)value, TRUE);
    }
	
	va_end( argp );
	
	sprintf( tmp, "Event '%s' sent to object '%s' at %p", event, object->type, object );

call_handlers:

    e.handled = 0;

    len = claro_list_count(object->event_handlers);

    //this should all be in a hash table too!
    for(i = 0; i < len; i++)
    {
        event_iface_func_t *iff;
		event_t *ep = &e;
		
		h = (event_handler_t *)claro_list_get_item(object->event_handlers, i);
		
		if ( strcmp( event, h->type ) )
			continue;
		
		if ( h->data != NULL )
		{
			iff = (event_iface_func_t *)h->func;
			(*iff)( object, ep, h->data );
		}
		else
			(*h->func)( object, ep );
    }

	/* debug for everything but mainloop, which is called too often to debug! */
    if(!mainloop)    
        clog( CL_DEBUG, "%s, %d handlers called.", tmp, hn );
	
    if(e.args)
	    claro_hashtable_unref( e.args );

	return e.handled;
}

void *event_get_ptr( event_t *e, const char *key )
{
	claro_typebox_t *value = claro_hashtable_lookup( e->args, key );
	cassert( value != NULL, "Event hashtable does not contain the key '%s'", key );
	return claro_typebox_extract_pointer( value );
}

int event_get_int( event_t *e, const char *key )
{
	claro_typebox_t *value = claro_hashtable_lookup( e->args, key );
	cassert( value != NULL, "Event hashtable does not contain the key '%s'", key );
	return claro_typebox_extract_int( value );
}

double event_get_double( event_t *e, const char *key )
{
	claro_typebox_t *value = claro_hashtable_lookup( e->args, key );
	cassert( value != NULL, "Event hashtable does not contain the key '%s'", key );
	return claro_typebox_extract_double( value );
}

void object_addhandler( object_t *object, const char *event, event_func_t *func )
{
//	node_t *n;
	event_handler_t *h;
	
//	n = node_create( );
	h = (event_handler_t *) g_malloc0( sizeof(event_handler_t) );
	
	strncpy( h->type, event, 32 );
	h->func = func;

    claro_list_append(object->event_handlers, (void *)h);	
    
	/* add to object's event list */
//	node_add( h, n, &object->event_handlers );
}

void object_addhandler_interface( object_t *object, const char *event, event_iface_func_t *func, void *data )
{
//	node_t *n;
	event_handler_t *h;
	
//	n = node_create( );
	h = (event_handler_t *) g_malloc0( sizeof(event_handler_t) );
	
	strncpy( h->type, event, 32 );
	h->func = (event_func_t*) func;
	h->data = data;
	
	/* add to object's event list */
//	node_add( h, n, &object->event_handlers );
    
    claro_list_append(object->event_handlers, (void *)h);	    
}

const char *event_get_name( event_t *event )
{
	return event->name;
}
 
