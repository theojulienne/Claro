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

#ifndef uchar
#define uchar unsigned char
#endif

#define cLelexVerticalAlignTop		0x01
#define cLelexVerticalAlignMiddle	0x02
#define cLelexVerticalAlignBottom	0x03

#define cLelexCellMinimum 0x01

typedef struct
{
	int size;
	uchar is_percentage;
	uchar is_set;
} sizeinfo_t;

typedef struct
{
	char name[32];
	
	bounds_t bounds;
	
	sizeinfo_t width;
	sizeinfo_t height;
	short valign;
	short flags;
} lelex_cell_t;

typedef struct
{
	lelex_cell_t *cells;
	int numcells;
	
	sizeinfo_t height;
} lelex_row_t;

typedef struct
{
	lelex_row_t *rows;
	lelex_cell_t *cells;
	int numrows;
	int numcells;
} lelex_t;

CLFEXP lelex_t *lelex_parse( const char *layout );
CLFEXP void lelex_destroy( lelex_t *lel );
CLFEXP void lelex_calculate( lelex_t *lel, int width, int height, int min_cell_width, int min_cell_height );
CLFEXP lelex_cell_t *lelex_get_cell( lelex_t *lel, const char *name );
CLFEXP void sizeinfo_set( sizeinfo_t *ptr, int size, int percentage );
