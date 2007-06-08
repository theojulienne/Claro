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

// kinda stupid test..."borrowed" from glib test-suite

int main( int argc, char *argv[] )
{
    int atomic = 0;
	
    claro_base_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );

    g_assert (claro_atomic_add_xfer_int(&atomic, 5) == 0);
    g_assert (atomic == 5);

    g_assert (claro_atomic_add_xfer_int(&atomic, -10) == 5);
    g_assert (atomic == -5);


    claro_atomic_add_int(&atomic, 20);
    g_assert (atomic == 15);

    claro_atomic_add_int(&atomic, -35);
    g_assert (atomic == -20);

    g_assert (atomic == claro_atomic_get_int(&atomic));

    g_assert (claro_atomic_compare_xfer_int(&atomic, -20, 20));
    g_assert (atomic == 20);
  
    g_assert (!claro_atomic_compare_xfer_int(&atomic, 42, 12));
    g_assert (atomic == 20);
  
    g_assert (claro_atomic_compare_xfer_int(&atomic, 20, G_MAXINT));
    g_assert (atomic == G_MAXINT);

    g_assert (claro_atomic_compare_xfer_int(&atomic, G_MAXINT, G_MININT));
    g_assert (atomic == G_MININT);

    clog( CL_INFO, "%s SUCCESS", __FILE__ );

    return 0;
}

