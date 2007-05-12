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

int event_send( object_t *object, const char *event, const char *fmt, ... )
{
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
}

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

void object_addhandler( object_t *object, const char *event, event_func_t *func )
{
	node_t *n;
	event_handler_t *h;
	
	n = node_create( );
	h = (event_handler_t *)smalloc( sizeof(event_handler_t) );
	
	strncpy( h->type, event, 32 );
	h->func = func;
	
	/* add to object's event list */
	node_add( h, n, &object->event_handlers );
}

void object_addhandler_interface( object_t *object, const char *event, event_func_t *func, void *data )
{
	node_t *n;
	event_handler_t *h;
	
	n = node_create( );
	h = (event_handler_t *)smalloc( sizeof(event_handler_t) );
	
	strncpy( h->type, event, 32 );
	h->func = func;
	h->data = data;
	
	/* add to object's event list */
	node_add( h, n, &object->event_handlers );
}

char *event_get_name( event_t *event )
{
	return event->name;
}
