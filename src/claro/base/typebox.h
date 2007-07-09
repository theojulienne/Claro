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


#ifndef _CLARO_BASE_TYPEBOX_H
#define _CLARO_BASE_TYPEBOX_H

typedef struct _claro_typebox claro_typebox_t;

/*****************************************************************************
 * claro_typebox_create
 *     
 *  Creates a box and places a type inside, referencing as needed.
 *
 * @name                    claro_typebox_create
 * @param   type_code       the 1-byte character representing the type
 * @param   ...             the value of the specified type to box
 * @return                  newly created type box or NULL on failure
 */
 
CLFEXP claro_typebox_t *claro_typebox_create( char type_code, ... );


/*****************************************************************************
 * claro_typebox_destroy
 *     
 *  Destroys a typebox and unreferences any objects
 *
 * @name                    claro_typebox_destroy
 * @param   box             a pointer to a valid typebox
 */

CLFEXP void claro_typebox_destroy( claro_typebox_t *box );


/*****************************************************************************
 * claro_typebox_extract_pointer
 *     
 *  Returns the value contained in a box as a pointer
 *
 * @name                    claro_typebox_extract_pointer
 * @param   box             a pointer to a valid typebox
 * @return                  the value as a pointer
 */

CLFEXP void *claro_typebox_extract_pointer( claro_typebox_t *box );


/*****************************************************************************
 * claro_typebox_extract_int
 *     
 *  Returns the value contained in a box as an int
 *
 * @name                    claro_typebox_extract_int
 * @param   box             a pointer to a valid typebox
 * @return                  the value as an int
 */

CLFEXP int claro_typebox_extract_int( claro_typebox_t *box );


/*****************************************************************************
 * claro_typebox_extract_double
 *     
 *  Returns the value contained in a box as a double
 *
 * @name                    claro_typebox_extract_double
 * @param   box             a pointer to a valid typebox
 * @return                  the value as a double
 */

CLFEXP double claro_typebox_extract_double( claro_typebox_t *box );

#endif

