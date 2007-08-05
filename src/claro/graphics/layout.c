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


#include <claro/graphics.h>

bounds_t *lt_bounds(layout_t *lt, const char *name) 
{
	lelex_cell_t *cell = lelex_get_cell(lt->lelex, name);
	
	if ( cell )
	{
		cell->bounds.owner = OBJECT(lt);
		return &cell->bounds;
	}
	
	return NULL;
}


void layout_recalculate_event( object_t *obj, event_t *event )
{
	widget_t *parent;
	layout_t *lt = (layout_t *)obj;
	
	parent = (widget_t *)obj->parent;
	
	lelex_calculate( lt->lelex, parent->size_ct.w, parent->size_ct.h, lt->min_w, lt->min_h );
}

claro_define_type_partial( layout, object, NULL, NULL, NULL, NULL );

layout_t *layout_create_i( object_t *parent )
{
	object_t *object;

	object = object_create_from_class( layout_type, parent );

	object_realize( object );

	return (layout_t *)object;
}

layout_t *layout_create( object_t *parent, const char *layout_spec, bounds_t bounds, int min_w, int min_h )
{
	layout_t *lt = layout_create_i( parent );
	
	assert(lt != NULL && "object create failed");
	
	lt->min_w = min_w;
	lt->min_h = min_h;
	lt->bounds = bounds;
	
	lt->lelex = lelex_parse( layout_spec );
	
	/* recalculate on parent's resize */
	object_addhandler( OBJECT(lt), "update", layout_recalculate_event );
	//object_addhandler( OBJECT(lt), "parent_attach", layout_parentsize_handle );
	layout_recalculate_event( OBJECT(lt), NULL );
	
	return lt;
}

void layout_destroy(layout_t *lt) 
{
	lelex_destroy( lt->lelex );
	free(lt);
}


void layout_cell_set_w(layout_t *lt, const char *name, int w) 
{
	lelex_cell_t *cell = lelex_get_cell( lt->lelex, name );
	
	cassert( w >= 0, "width must be >= 0" );

	sizeinfo_set( &cell->width, w, 0 );
}

void layout_cell_flex_w(layout_t *lt, const char *name) 
{
	lelex_cell_t *cell = lelex_get_cell( lt->lelex, name );
	
	cell->width.is_set = 0;
}



void layout_cell_flex_h(layout_t *lt, const char *name) 
{
	lelex_cell_t *cell = lelex_get_cell( lt->lelex, name );
	
	cell->height.is_set = 0;
}


void layout_cell_set_h(layout_t *lt, const char *name, int h)
{
	lelex_cell_t *cell = lelex_get_cell( lt->lelex, name );
	
	cassert( h >= 0, "height must be >= 0" );
	
	sizeinfo_set( &cell->height, h, 0 );
}

