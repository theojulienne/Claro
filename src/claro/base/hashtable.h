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


#ifndef _CLARO_BASE_HASHTABLE_H
#define _CLARO_BASE_HASHTABLE_H

typedef struct _claro_hashtable claro_hashtable_t;

typedef void hashtable_foreach_func(const void * key, const void * value, void * user_arg);

//PRIVATE
void _claro_hashtable_init();

/*****************************************************************************
 * claro_hashtable_str_create
 *     
 *  You MUST use ALL static or ALL dynamic strings for keys here. 
 *  Mixing them will result in very bad behavior. If you need to add a static 
 *  string, use sstrdup().
 *
 * @name                    claro_hashtable_str_create
 * @param   copy_strings    whether to copy all string keys, or to use static strings
 * @param   val_free_fn     function for freeing values or NULL
 * @return                  newly created hashtable or NULL on failure
 */
 
CLFEXP claro_hashtable_t *
claro_hashtable_str_create(bool_t copy_strings, void (*val_free_fn) (void*));

/*****************************************************************************
 * claro_hashtable_create
   
 * @name                    claro_hashtable_create
 * @param   hash_fn         function for hashing keys
 * @param   key_eq_fn       function for determining key equality
 * @param   key_free_fn     function for freeing keys or NULL
 * @param   val_free_fn     function for freeing values or NULL
 * @return                  newly created hashtable or NULL on failure
 */

CLFEXP claro_hashtable_t *
claro_hashtable_create(unsigned int (*hash_fn) (const void*),
                        bool_t (*key_eq_fn) (const void*,const void*),
				        void (*key_free_fn) (void*),
				        void (*val_free_fn) (void*));

/*****************************************************************************
 * claro_hashtable_insert
   
 * @name                claro_hashtable_insert
 * @param   hashtable   the hashtable to use
 * @param   key         key to use- will be freed upon removal/replacement
 * @param   value       value to use- will be freed upon removal/replacement- must exist during the lifetime of the hashtable
 * @param   replace     replace the key with a new value ? (freeing the old if free_val_fn was supplied)
 */

CLFEXP void 
claro_hashtable_insert(claro_hashtable_t * hashtable, void * key, void * value, bool_t replace);


/*****************************************************************************
 * claro_hashtable_lookup
   
 * @name                claro_hashtable_lookup
 * @param   hashtable   the hashtable to use
 * @param   key         the key to use
 * @return              the associated value, or NULL if not found
 */

CLFEXP void *
claro_hashtable_lookup(claro_hashtable_t * hashtable, const void * key);

/*****************************************************************************
 * claro_hashtable_remove
   
 * @name                claro_hashtable_remove
 * @param   hashtable   the hashtable to use
 * @param   key         the key to use
 * @return              true if the value was successfully found and removed
 */

CLFEXP bool_t
claro_hashtable_remove(claro_hashtable_t * hashtable, const void * key);

/*****************************************************************************
 * claro_hashtable_count
   
 * @name                claro_hashtable_count
 * @param   hashtable   the hashtable to use
 * @return              the number of elements in the hashtable
 */
CLFEXP unsigned int
claro_hashtable_count(claro_hashtable_t * hashtable);


/*****************************************************************************
 * claro_hashtable_unref
   
 * @name                claro_hashtable_unref
 * @param   hashtable   the hashtable to use
 */

CLFEXP void
claro_hashtable_unref(claro_hashtable_t * hashtable);

/*****************************************************************************
 * claro_hashtable_ref
   
 * @name                claro_hashtable_ref
 * @param   hashtable   the hashtable to use
 * @return              the hashtable
 */

CLFEXP claro_hashtable_t *
claro_hashtable_ref(claro_hashtable_t * hashtable);

/*****************************************************************************
 * claro_hashtable_iter_keys
   
 * @name                claro_hashtable_iter_keys
 * @param   hashtable   the hashtable to use
 * @param   iter_func   the key iteration callback
 * @param   arg         the user argument to pass in
 * @return              the hashtable
 */

CLFEXP void 
claro_hashtable_foreach(claro_hashtable_t * hashtable, hashtable_foreach_func * foreach_func, void * arg);

/*****************************************************************************
 * claro_hashtable_get_keys
   
 * @name                claro_hashtable_ref
 * @param   hashtable   the hashtable to use
 * @return              the hashtable
 */

CLFEXP claro_list_t * 
claro_hashtable_get_keys(claro_hashtable_t * hashtable);

#endif

