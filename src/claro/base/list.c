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

CLFEXP claro_list_t * claro_list_create()
{
    claro_list_t * list = (claro_list_t *) g_ptr_array_new();
    g_assert(list != NULL);
    return list;
}

CLFEXP void claro_list_destroy(claro_list_t * list)
{
    g_ptr_array_free((GPtrArray *)list, TRUE);
}

CLFEXP void claro_list_append(claro_list_t * list, void * item)
{
    g_ptr_array_add((GPtrArray *)list, item);
}

CLFEXP bool_t claro_list_remove(claro_list_t * list, void * item)
{
    return g_ptr_array_remove((GPtrArray *)list, item);
}

CLFEXP void * claro_list_get_item(claro_list_t * list, int index)
{
    GPtrArray * array = (GPtrArray *)list;
    g_return_val_if_fail(array != NULL, NULL);
	g_return_val_if_fail(index >= 0 || index < array->len, NULL);
    return g_ptr_array_index(array, index);
}

CLFEXP void claro_list_set_item(claro_list_t * list, int index, void * item)
{
    GPtrArray * array = (GPtrArray *)list;
    g_return_if_fail(array != NULL);
	g_return_if_fail(index >= 0 || index < array->len);
    g_ptr_array_index(array, index) = item;
}

CLFEXP unsigned int claro_list_count(claro_list_t * list)
{
    GPtrArray * array = (GPtrArray *)list;
    g_return_val_if_fail(array != NULL, 0);
    return array->len;
}


