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

struct _claro_hashtable
{
    claro_boxed_t base;
    GHashTable * table;    
};

static void _claro_hashtable_finalize(void * object)
{
    claro_hashtable_t * hash = (claro_hashtable_t *)object;

    g_return_if_fail(hash != NULL);
    
    g_hash_table_destroy(hash->table);    
}

static claro_boxed_type_t * _hashtable_type;

void _claro_hashtable_init()
{
    _hashtable_type = claro_boxed_register("claro_hashtable_t", _claro_hashtable_finalize, sfree, NULL);
}
 
CLFEXP claro_hashtable_t *
claro_hashtable_str_create(bool_t copy_strings, void (*val_free_fn) (void*))
{
    return claro_hashtable_create(g_str_hash, g_str_equal, 
        copy_strings ? g_free : NULL, val_free_fn);    
}

CLFEXP claro_hashtable_t *
claro_hashtable_create(unsigned int (*hash_fn) (const void*),
                        bool_t (*key_eq_fn) (const void*,const void*),
				        void (*key_free_fn) (void*),
				        void (*val_free_fn) (void*))
{
    claro_hashtable_t * hash = (claro_hashtable_t *)smalloc(sizeof(claro_hashtable_t));
    
    g_assert(_hashtable_type != NULL);

    hash->base._type = _hashtable_type;
    hash->base.ref_count = 1;

    hash->table = g_hash_table_new_full((GHashFunc)hash_fn, (GEqualFunc)key_eq_fn, 
        (GDestroyNotify)key_free_fn, (GDestroyNotify)val_free_fn);

    g_assert(hash->table != NULL);
    
    return hash;
}

CLFEXP void 
claro_hashtable_insert(claro_hashtable_t * hashtable, void * key, void * value, bool_t replace)
{
#ifdef NEEDS_GLIB
     g_hash_table_insert_replace(hashtable->table, key, value, replace);    
#else
    if(replace)
        g_hash_table_replace(hashtable->table, key, value);
    else
        g_hash_table_insert(hashtable->table, key, value);
#endif  
}

CLFEXP void *
claro_hashtable_lookup(claro_hashtable_t * hashtable, const void * key)
{
    return g_hash_table_lookup(hashtable->table, key);
}

CLFEXP bool_t
claro_hashtable_remove(claro_hashtable_t * hashtable, const void * key)
{
    return g_hash_table_remove(hashtable->table, key);
}

CLFEXP unsigned int
claro_hashtable_count(claro_hashtable_t * hashtable)
{
    return g_hash_table_size(hashtable->table);
}

CLFEXP void
claro_hashtable_unref(claro_hashtable_t * hashtable)
{
    claro_boxed_unref(hashtable);
}

CLFEXP claro_hashtable_t *
claro_hashtable_ref(claro_hashtable_t * hashtable)
{
    return claro_boxed_ref(hashtable);
}

