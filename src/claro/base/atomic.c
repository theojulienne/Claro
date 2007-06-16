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

#if defined(REAL_GLIB)

void _claro_atomic_init()
{
}

#elif defined(_MAC) && defined(__i386__) || defined(_WIN32)

void _claro_atomic_init()
{
}

// these could really be better..

int claro_atomic_get_int(volatile int * ptr)
{
    return *ptr;
}

void claro_atomic_set_int(volatile int * ptr, int new_val)
{
    *ptr = new_val;
}

void claro_atomic_add_int(volatile int * ptr, int val)
{
    __asm__ __volatile__ ("lock; addl %1, %0"
        : "=m" (*ptr) 
		: "ir" (val), "m" (*ptr));
}

int  claro_atomic_add_xfer_int(volatile int * ptr, int val)
{
    int res;

    __asm__ __volatile__ ("lock; xaddl %0, %1"
        : "=r" (res), "=m" (*ptr) 
        : "0" (val), "m" (*ptr));

    return res;
}

int claro_atomic_compare_xfer_int(volatile int	* ptr, int old_val, int new_val)
{
    int res;
 
    __asm__ __volatile__ ("lock; cmpxchgl %2, %1"
        : "=a" (res), "=m" (*ptr)
		: "r" (new_val), "m" (*ptr), "0" (old_val)); 

    return res == old_val;
}

/*
#elif defined(__ppc__) && defined(_MAC)
#error "FIXME: someone should implement and test PPC atomic routines"
*/
#else

static pthread_mutex_t _atomic_mutex;

void _claro_atomic_init()
{
    int res;
    
    res = pthread_mutex_init(&_atomic_mutex, NULL);

    if(res != 0)
        clog(CL_ERROR, 
}

#endif






