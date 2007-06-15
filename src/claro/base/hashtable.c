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


CLFEXP claro_hashtable_t *
claro_hashtable_str_create(bool_t copy_strings, void (*val_free_fn) (void*))
{
    claro_hashtable_t * hash = (claro_hashtable_t*)g_hash_table_new_full(g_str_hash, 
        g_str_equal, copy_strings ? g_free : NULL, (GDestroyNotify)val_free_fn);    
    g_assert(hash != NULL);
    return hash;
}

CLFEXP claro_hashtable_t *
claro_hashtable_create(unsigned int (*hash_fn) (const void*),
                        bool_t (*key_eq_fn) (const void*,const void*),
				        void (*key_free_fn) (void*),
				        void (*val_free_fn) (void*))
{
    claro_hashtable_t * hash = (claro_hashtable_t *)g_hash_table_new_full((GHashFunc)hash_fn, 
        (GEqualFunc)key_eq_fn, (GDestroyNotify)key_free_fn, (GDestroyNotify)val_free_fn);
    g_assert(hash != NULL);
    return hash;
}

CLFEXP void 
claro_hashtable_insert(claro_hashtable_t * hashtable, void * key, void * value, bool_t replace)
{
#ifdef NEEDS_GLIB
     g_hash_table_insert_replace((GHashTable *)hashtable, key, value, replace);    
#else
    if(replace)
        g_hash_table_replace((GHashTable *)hashtable, key, value);
    else
        g_hash_table_insert((GHashTable *)hashtable, key, value);
#endif  
}

CLFEXP void *
claro_hashtable_lookup(claro_hashtable_t * hashtable, const void * key)
{
    return g_hash_table_lookup((GHashTable *)hashtable, key);
}

CLFEXP bool_t
claro_hashtable_remove(claro_hashtable_t * hashtable, const void * key)
{
    return g_hash_table_remove((GHashTable *)hashtable, key);
}

CLFEXP unsigned int
claro_hashtable_count(claro_hashtable_t * hashtable)
{
    return g_hash_table_size((GHashTable *)hashtable);
}

CLFEXP void
claro_hashtable_destroy(claro_hashtable_t * hashtable)
{
    g_hash_table_destroy((GHashTable *)hashtable);
}

