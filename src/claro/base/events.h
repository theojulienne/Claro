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

#ifndef _CLARO_BASE_EVENTS_H
#define _CLARO_BASE_EVENTS_H

typedef struct event_
{
	object_t *object; /* the object which this event was sent to */
	char name[64];

	int handled;

	int arg_num;     /* number of arguments */
	char format[16]; /* format of the arguments sent */

	void **arglist;  /* list of args, as per format. */
} event_t;

typedef void event_func_t( object_t *object, event_t *event );
typedef void event_iface_func_t( object_t *object, event_t *event, void *data );

typedef struct event_handler_
{
	char type[32];

	void *data;

	event_func_t *func; /* the function that handles this event */
} event_handler_t;

#define event_handler(n) void n ( object_t *object, event_t *event )

CLFEXP void *event_get_arg_ptr( event_t *e, int arg );
CLFEXP double event_get_arg_double( event_t *e, int arg );
CLFEXP int event_get_arg_int( event_t *e, int arg );

/* event functions */

CLFEXP void object_addhandler( object_t *object, const char *event, event_func_t *func );
CLFEXP void object_addhandler_interface( object_t *object, const char *event, event_func_t *func, void *data );
CLFEXP int event_send( object_t *object, const char *event, const char *fmt, ... );

CLFEXP char *event_get_name( event_t *event );

#endif
