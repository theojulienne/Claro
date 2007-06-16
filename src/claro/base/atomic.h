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


#ifndef _CLARO_BASE_ATOMIC_H
#define _CLARO_BASE_ATOMIC_H

#ifdef REAL_GLIB

#define claro_atomic_get_int    g_atomic_int_get
#define claro_atomic_set_int    g_atomic_int_set
#define claro_atomic_add_int    g_atomic_int_add
#define claro_atomic_add_xfer_int   g_atomic_int_exchange_and_add
#define claro_atomic_compare_xfer_int   g_atomic_int_compare_and_exchange    

#else

int claro_atomic_get_int(volatile int * ptr);

void claro_atomic_set_int(volatile int * ptr, int new_val);

void claro_atomic_add_int(volatile int * ptr, int val);

int  claro_atomic_add_xfer_int(volatile int * ptr, int val);

bool_t claro_atomic_compare_xfer_int(volatile int	* ptr, int old_val, int new_val);

#endif

//_CLARO_BASE_ATOMIC_H
#endif

