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


#ifndef _CLARO_BASE_BOXED_H
#define _CLARO_BASE_BOXED_H

typedef struct _claro_boxed_type claro_boxed_type_t;

typedef struct
{
    claro_boxed_type_t * _type;
    volatile int ref_count;
} claro_boxed_t;

claro_boxed_type_t * claro_boxed_register(const char * type_name, 
    void (* finalize_fn) (void *),
    void (* destroy_fn) (void *), bool_t * success);

void * claro_boxed_ref(void * object);

void claro_boxed_unref(void * object);

const char * claro_boxed_get_type_name(void * object);

//PRIVATE
void _claro_boxed_init();

#endif
