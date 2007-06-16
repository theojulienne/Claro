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
 
#ifndef _CLARO_BASE_MUTEX_H
#define _CLARO_BASE_MUTEX_H

#ifdef _WIN32

#include <windows.h>

typedef CRITICAL_SECTION claro_mutex_t;

#define	claro_mutex_create		InitializeCriticalSection
#define	claro_mutex_lock		EnterCriticalSection
#define claro_mutex_unlock		LeaveCriticalSection
#define	claro_mutex_destroy		DeleteCriticalSection

#else

#include <pthread.h>

typedef pthread_mutex_t claro_mutex_t;

#define claro_mutex_create(mutex) pthread_mutex_init(mutex, NULL)
#define	claro_mutex_lock		pthread_mutex_lock
#define claro_mutex_unlock		pthread_mutex_unlock
#define claro_mutex_destroy		pthread_mutex_destroy

#endif

#endif
	
			
