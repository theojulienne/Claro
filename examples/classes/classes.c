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

#include <assert.h>
#include <claro/base.h>
#include "foo.h"

int main( int argc, char *argv[] )
{
    claro_hashtable_t * hash;
    char buf[256];
    int i;

	claro_base_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
    // hashtable test 

    hash = claro_hashtable_str_create(FALSE, NULL);
    assert(hash != NULL);    

    claro_hashtable_insert(hash, "one", (void*)0x1, FALSE);
    claro_hashtable_insert(hash, "two", (void*)0x2, FALSE);
    claro_hashtable_insert(hash, "three", (void*)0x3, FALSE);

    assert(claro_hashtable_count(hash) == 3);
    
    assert(claro_hashtable_lookup(hash, "one") == (void*)0x1);
    assert(claro_hashtable_lookup(hash, "two") == (void*)0x2);
    assert(claro_hashtable_lookup(hash, "three") == (void*)0x3);

    assert(claro_hashtable_count(hash) == 3);

    assert(claro_hashtable_remove(hash, "one") == TRUE);
    assert(claro_hashtable_remove(hash, "two") == TRUE);
    assert(claro_hashtable_remove(hash, "three") == TRUE);
    
    assert(claro_hashtable_count(hash) == 0);    
    
    claro_hashtable_destroy(hash);

    hash = claro_hashtable_str_create(TRUE, NULL);
    assert(hash != NULL);    

    for(i = 0; i < 100; i++)
    {
        sprintf(buf, "%d", i);
        claro_hashtable_insert(hash, sstrdup(buf), (void*)i, FALSE);
    }    

    for(i = 0; i < 100; i++)
    {
        sprintf(buf, "%d", i);
        assert(claro_hashtable_lookup(hash, buf) == (void*)i);
    }

    assert(claro_hashtable_count(hash) == 100);
    claro_hashtable_destroy(hash);

	object_t *foo = foo_create( NULL );
	
	//block_heap_loginfo( );
	object_destroy( foo );
	
	claro_loop( );
	
	return 0;
}
