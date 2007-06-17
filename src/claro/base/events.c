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

#ifndef OLD_EVENTS

/* perhaps add free value function pointer here */
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
#endif

int event_send( object_t *object, const char *event, const char *fmt, ... )
{
#ifndef OLD_EVENTS
	va_list argp;
//	node_t *n;
	event_handler_t *h;
	event_t e;
	int hn = 0;
	char tmp[1024];
	int a, i, len;
    bool_t mainloop;
    int int_num = 0;
	int dbl_num = 0;
    int * ints = NULL;
	double * dbls = NULL; 
	int b;

	strcpy( e.name, event );
	e.object = object;
	e.arg_num = strlen( fmt );
	strncpy( e.format, fmt, 16 );
	
    mainloop = strcmp( event, "mainloop" ) == 0 ? TRUE : FALSE;

    e.args = NULL;

    if(mainloop)
        goto call_handlers;
    
    e.args = claro_hashtable_str_create(TRUE, _free_event_arg);

	for ( a = 0; a < e.arg_num; a++ )
	{
		if ( fmt[a] == 'i' ) int_num++;
		if ( fmt[a] == 'd' ) dbl_num++;
	}
	
	ints = (int *) g_malloc0(int_num * sizeof(int));
	dbls = (double *) g_malloc0(dbl_num * sizeof(double));
	
    va_start( argp, fmt );

	for ( a = 0; a < e.arg_num; a++ )
	{
		char *name = va_arg( argp, char * );
		event_arg_t * value = g_new0(event_arg_t, 1);
        value->type = fmt[a];			

		switch ( fmt[a] )
		{
			case 'p':				
                value->val = va_arg( argp, void * );			
                break;
			case 'i':
				b = int_num - 1;
				ints[b] = va_arg( argp, int );
				value->val = (gpointer) &ints[b];
				int_num = b;
				break;
			case 'd':
				b = dbl_num - 1;
				dbls[b] = va_arg( argp, double );
				value->val = (gpointer) &dbls[b];
				dbl_num = b;
				break;
			default:
				value->val = NULL; /* this is bad. */
		}

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
	
    /*
	LIST_FOREACH( n, object->event_handlers.head )
	{
		event_iface_func_t *iff;
		event_t *ep = &e;
		
		h = (event_handler_t *)n->data;
		
		if ( strcmp( event, h->type ) )
			continue;
		
		if ( h->data != NULL )
		{
			iff = (event_iface_func_t *)h->func;
			(*iff)( object, ep, h->data );
		}
		else
			(*h->func)( object, ep );
		
		hn++;
	}
	*/

	/* debug for everything but mainloop, which is called too often to debug! */
    if(!mainloop)    
        clog( CL_DEBUG, "%s, %d handlers called.", tmp, hn );
	
    if(e.args)
	    claro_hashtable_unref( e.args );

    if(ints)
        g_free(ints);
    
    if(dbls)
        g_free(dbls);	

	return e.handled;

#else
	va_list argp;
	node_t *n;
	event_handler_t *h;
	event_t e;
	int hn = 0;
	char tmp[1024];
	int a;

	va_start( argp, fmt );

	strcpy( e.name, event );
	e.object = object;
	e.arg_num = strlen( fmt );
	strncpy( e.format, fmt, 16 );
	
	e.arglist = NULL;
	if ( e.arg_num > 0 )
		e.arglist = (void **)malloc( sizeof(void *) * e.arg_num );
	
	for ( a = 0; a < e.arg_num; a++ )
	{
		switch ( fmt[a] )
		{
			case 'p':
				e.arglist[a] = va_arg( argp, void * );
				break;
			case 'i':
				e.arglist[a] = malloc( sizeof(int) );
				*((int *)e.arglist[a]) = va_arg( argp, int );
				break;
			case 'd':
				e.arglist[a] = malloc( sizeof(double) );
				*((double *)e.arglist[a]) = va_arg( argp, double );
				break;
			default:
				e.arglist[a] = NULL; /* this is bad. */
		}
	}
	
	e.handled = 0;

	va_end( argp );
	
	sprintf( tmp, "Event '%s' sent to object '%s' at %p", event, object->type, object );
	
	LIST_FOREACH( n, object->event_handlers.head )
	{
		event_iface_func_t *iff;
		event_t *ep = &e;
		
		h = (event_handler_t *)n->data;
		
		if ( strcmp( event, h->type ) )
			continue;
		
		if ( h->data != NULL )
		{
			iff = (event_iface_func_t *)h->func;
			(*iff)( object, ep, h->data );
		}
		else
			(*h->func)( object, ep );
		
		hn++;
	}
	
	for ( a = 0; a < e.arg_num; a++ )
	{
		if ( fmt[a] == 'i' || fmt[a] == 'd' )
			free( e.arglist[a] );
	}
	
	if ( e.arglist != NULL )
		free( e.arglist );
	
	/* debug for everything but mainloop, which is called too often to debug! */
	if ( strcmp( event, "mainloop" ) )
		clog( CL_DEBUG, "%s, %d handlers called.", tmp, hn );
	
	return e.handled;
#endif
}

#ifndef OLD_EVENTS
void *event_get_ptr( event_t *e, const char *key )
{
	event_arg_t * value = claro_hashtable_lookup( e->args, key );
	return value->val;
}

int event_get_int( event_t *e, const char *key )
{
	int *a = (int *)event_get_ptr( e, key );
	
	if ( a == NULL )
		return 0;
	
	return *a;
}

double event_get_double( event_t *e, const char *key )
{
	double *a = (double *)event_get_ptr( e, key );
	
	if ( a == NULL )
		return 0;
	
	return *a;
}

#else
void *event_get_arg_ptr( event_t *e, int arg )
{
	if ( arg >= e->arg_num || arg < 0 )
		return NULL;
	
	return e->arglist[arg];
}

int event_get_arg_int( event_t *e, int arg )
{
	int *a = (int *)event_get_arg_ptr( e, arg );
	
	if ( a == NULL )
		return 0;
	
	return *a;
}

double event_get_arg_double( event_t *e, int arg )
{
	double *a = (double *)event_get_arg_ptr( e, arg );
	
	if ( a == NULL )
		return 0;
	
	return *a;
}
#endif

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
 
