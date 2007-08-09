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


#ifndef _CLARO_BASE_LIST_H
#define _CLARO_BASE_LIST_H

typedef void claro_list_t;

typedef int list_compare_func(const void * value1, const void * value2, void * user_arg);

CLFEXP claro_list_t * claro_list_create();

CLFEXP void claro_list_destroy(claro_list_t * list);

CLFEXP void claro_list_append(claro_list_t * list, void * item);

CLFEXP bool_t claro_list_remove(claro_list_t * list, void * item);

CLFEXP void * claro_list_get_item(claro_list_t * list, int index);

CLFEXP void claro_list_set_item(claro_list_t * list, int index, void * item);

CLFEXP unsigned int claro_list_count(claro_list_t * list);

#define LIST_LENGTH claro_list_count

CLFEXP void claro_list_reverse(claro_list_t * list);

CLFEXP void claro_list_sort(claro_list_t * list, 
    list_compare_func * compare_func, void * user_arg);

CLFEXP void claro_list_insert(claro_list_t * list, int index, void * data);

#endif

