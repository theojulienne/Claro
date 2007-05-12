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


#ifndef _CLARO_CLASS_EXAMPLE_FOO_H
#define _CLARO_CLASS_EXAMPLE_FOO_H

/* definition of the foo class, inherits from object */
typedef struct foo_ foo_t;

declare_class( foo );
struct foo_
{
	object_t object;
};

/* foo class function(s) */
object_t *foo_create( object_t *parent );

#endif
