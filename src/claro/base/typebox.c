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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include <claro/base.h>

struct _claro_typebox
{
	union
	{
		gpointer val;
		int ival;
		double dval;
	} value;
	
	unsigned char type;
};

claro_typebox_t *claro_typebox_create( char type_code, ... )
{
	claro_typebox_t *box = g_new0(claro_typebox_t, 1);
	va_list argp;
	
	box->type = type_code;
	
	va_start( argp, type_code );
	
	switch ( type_code )
	{
		case 'o': // object will end up being a b as well, right now this will crash
		case 'b':
			box->value.val = va_arg( argp, void * );
			claro_boxed_ref( box->value.val );
			break;
		case 'p':
			box->value.val = va_arg( argp, void * );
			break;
		case 'i':
			box->value.ival = va_arg( argp, int );
			break;
		case 'd':
			box->value.dval = va_arg( argp, double );
			break;
		default:
			g_free( box );
			va_end( argp );
			return NULL;
	}
	
	va_end( argp );
	
	return box;
}

void claro_typebox_destroy( claro_typebox_t *box )
{
	// unreference any 
	if ( box->type == 'o' || box->type == 'b' )
	{
		claro_boxed_unref( box->value.val );
	}
	
	free( box );
}

void *claro_typebox_extract_pointer( claro_typebox_t *box )
{
	cassert( box->type == 'p', "Attempt to extract a typebox value from '%c' to pointer is not supported.", box->type );
	return box->value.val;
}

int claro_typebox_extract_int( claro_typebox_t *box )
{
	cassert( box->type == 'i', "Attempt to extract a typebox value from '%c' to int is not supported.", box->type );
	return box->value.ival;
}

double claro_typebox_extract_double( claro_typebox_t *box )
{
	cassert( box->type == 'd', "Attempt to extract a typebox value from '%c' to double is not supported.", box->type );
	return box->value.dval;
}
