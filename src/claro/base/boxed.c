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

struct _claro_boxed_type
{
    const char * type_name;
    void (* finalize_fn) (void *);
    void (* destroy_fn) (void *);      
};

//Can't use claro hashtable here because the boxed wrapper won't exist yet .. :-(
static GHashTable * _boxed_type_table;

void _claro_boxed_init()
{
    _boxed_type_table = g_hash_table_new(g_str_hash, g_str_equal);
}

claro_boxed_type_t * claro_boxed_register(const char * type_name, 
    void (* finalize_fn) (void *),
    void (* destroy_fn) (void *), bool_t * success)
{
    claro_boxed_type_t * type = NULL;
    
    if(type_name == NULL)
    {
        if(success)
            *success = FALSE;
        return NULL;
    }

    type = g_hash_table_lookup(_boxed_type_table, type_name);

    if(type)
    {
        if(success)
            *success = FALSE;
        return type;
    }

    type = (claro_boxed_type_t *)smalloc(sizeof(claro_boxed_type_t));            
    
    type->type_name = type_name;
    type->finalize_fn = finalize_fn;
    type->destroy_fn = destroy_fn;

    g_hash_table_insert(_boxed_type_table, (void *)type_name, type);  

    if(success)
        *success = TRUE;

    clog(CL_DEBUG, "registered boxed type \"%s\"", type_name);

    return type;
}

void * claro_boxed_ref(void * object)
{
    claro_boxed_t * boxed = (claro_boxed_t *)object;
    g_return_val_if_fail(boxed != NULL, NULL);
    claro_atomic_add_int(&boxed->ref_count, 1);
    return object;
}

void claro_boxed_unref(void * object)
{
    int ref_count;
    claro_boxed_type_t * type;
    claro_boxed_t * boxed = (claro_boxed_t *)object;

    g_return_if_fail(boxed != NULL);
    g_return_if_fail(boxed->ref_count > 0);
    
    type = (claro_boxed_type_t *)boxed->_type;

    g_return_if_fail(type != NULL);

    ref_count = claro_atomic_get_int(&boxed->ref_count);
    
    if(ref_count > 1)
    {
        claro_atomic_set_int(&boxed->ref_count, ref_count-1);
    }
    else
    {
        g_return_if_fail(type != NULL);

        
        if(type->finalize_fn)
        {
            clog(CL_DEBUG, "finalizing boxed object of type \"%s\" at %p", type->type_name, object);        
            type->finalize_fn(object);
        }        
        
        if(type->destroy_fn)        
        {
            clog(CL_DEBUG, "freeing boxed object of type \"%s\" at %p", type->type_name, object);     
            type->destroy_fn(object);    
        }
    }   
}

const char * claro_boxed_get_type_name(void * object)
{
    claro_boxed_t * boxed = (claro_boxed_t *)object;
    claro_boxed_type_t * type;

    g_return_val_if_fail(boxed != NULL, NULL);

    type = boxed->_type;

    g_return_val_if_fail(type != NULL, NULL);  

    return type->type_name;
}
 
