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
#include <math.h>

void sizeinfo_set( sizeinfo_t *ptr, int size, int percentage )
{
	ptr->size = size;
	ptr->is_percentage = (uchar)percentage;
	ptr->is_set = 1;
	
	if ( ptr->size == 0 )
		ptr->is_set = 0;
}

void sizeinfo_set_from_string( sizeinfo_t *ptr, const char *str )
{
	int last = strlen( str ) - 1;
	char stmp[32];
	int percent = 0;
	
	strcpy( stmp, str );
	
	while ( last >= 0 && (str[last] == 'p' || str[last] == 'x') )
		last--;
	
	stmp[last+1] = 0;
	
	if ( stmp[last] == '%' )
	{
		stmp[last] = 0;
		percent = 1;
	}
	
	sizeinfo_set( ptr, atoi( stmp ), percent );
	
	//printf( "SIZE: %d (%d)\n", ptr->size, ptr->is_percentage );
}

void lelex_destroy( lelex_t *lel )
{
	// rows only contain pointers to the same memory block as the main ->cells
	
	free( lel->rows );
	free( lel->cells );
	free( lel );
}

lelex_t *lelex_parse( const char *layout )
{
	lelex_t *lel = NULL;
	const char *ptr;
	
	lel = g_new0(lelex_t, 1);
	
	lel->numrows = 0;
	lel->numcells = 0;
	
	for ( ptr = layout; *ptr != 0; ptr++ )
	{
		if ( *ptr == ']' )
			lel->numrows++;
		
		if ( *ptr == '|' || *ptr == ']' )
			lel->numcells++;
	}
	
	//printf( "%d rows, %d cells\n", lel->numrows, lel->numcells );
	
	// allocate all rows and all cells
	lel->rows = g_new0( lelex_row_t, lel->numrows );
	lel->cells = g_new0( lelex_cell_t, lel->numcells );
	
	// set row's cell pointers
	lelex_cell_t *cell_ptr = lel->cells;
	int row = 0;
	char cell_name[32] = "";
	int name_pos = 0;
	for ( ptr = layout; *ptr != 0; ptr++ )
	{
		if ( *ptr == ' ' || *ptr == '\t' )
			continue;
		
		//printf( "%c", *ptr );
		
		if ( *ptr == '[' )
		{
			// row has begin. set it's details
			lel->rows[row].cells = cell_ptr;
			lel->rows[row].numcells = 0;
			continue;
		}
		
		if ( *ptr == '|' || *ptr == ']' )
		{
			// cell has finished (through end of cell or row)
			strcpy( cell_ptr->name, cell_name );
			
			//printf( "CELL: %s\n", cell_name );
			
			lel->rows[row].numcells++;
			cell_ptr++;
			
			strcpy( cell_name, "" );
			name_pos = 0;
		}
		else if ( name_pos == 0 && *ptr == '_' )
		{
			sizeinfo_set_from_string( &lel->rows[row].height, "100%" );
			continue;
		}
		else if ( *ptr == '<' || *ptr == '>' )
		{
			cell_ptr->flags |= cLelexCellMinimum;
		}
		else if ( *ptr == '{' )
		{
			char tmp[16] = "0";
			int tmp_pos = 0;
			
			ptr++; // ignore the bracket
			
			// parse row properties
			while ( *ptr && *ptr != '}' )
			{
				if ( tmp_pos < 15 )
				{
					tmp[tmp_pos] = *ptr;
					tmp_pos++;
				}
				
				ptr++;
			}
			
			tmp[tmp_pos] = 0;
			
			sizeinfo_set_from_string( &lel->rows[row].height, tmp );
		}
		else if ( *ptr == '(' )
		{
			char tmp[16] = "0";
			int tmp_pos = 0;
			
			ptr++; // ignore the bracket
			
			// parse cell properties, starting with width
			while ( *ptr && *ptr != ')' && *ptr != ',' )
			{
				if ( tmp_pos < 15 )
				{
					tmp[tmp_pos] = *ptr;
					tmp_pos++;
				}
				
				ptr++;
			}
			
			tmp[tmp_pos] = 0;
			
			sizeinfo_set_from_string( &cell_ptr->width, tmp );
			
			// then the same for height, if it exists
			if ( *ptr == ',' )
			{
				tmp[0] = '0';
				tmp[1] = 0;
				tmp_pos = 0;
				
				ptr++; // ignore the comma
				
				while ( *ptr && *ptr != ')' )
				{
					if ( tmp_pos < 15 )
					{
						tmp[tmp_pos] = *ptr;
						tmp_pos++;
					}

					ptr++;
				}

				tmp[tmp_pos] = 0;

				sizeinfo_set_from_string( &cell_ptr->height, tmp );
				
				if ( lel->rows[row].height.is_percentage == 0 &&
					 lel->rows[row].height.size < cell_ptr->height.size )
				{
					lel->rows[row].height.size = cell_ptr->height.size;
					lel->rows[row].height.is_set = 1;
				}
			}
		}
		else
		{
			// limited to 32 chars including null char
			if ( name_pos == 31 )
				continue;
			
			cell_name[name_pos] = *ptr;
			name_pos++;
			
			cell_name[name_pos] = 0;
		}
		
		if ( *ptr == ']' )
		{
			//printf( "--- (row was %d%s height)\n", lel->rows[row].height.size, lel->rows[row].height.is_percentage?"%":"px" );
			
			// row has finished.
			row++;
		}
	}
	//printf( "\n" );
	
	return lel;
}

void lelex_calculate_cells( lelex_t *lel, lelex_row_t *row, int width, int height, int min_cell_width, int min_cell_height )
{
	int cell;
	int unknown_cells = 0;
	
	// the remaining width after statically widthed rows
	int available_width = width;
	
	// the total % of height requested from available
	int percentage_width = 0;
	int percentage_available = 0;
	
	// calculate how many unknown cells
	for ( cell = 0; cell < row->numcells; cell++ )
	{
		if ( row->cells[cell].flags & cLelexCellMinimum )
		{
			row->cells[cell].width.is_set = 1;
			row->cells[cell].width.is_percentage = 0;
			row->cells[cell].width.size = min_cell_width;
		}
		
		if ( row->cells[cell].width.is_set )
		{
			if ( row->cells[cell].width.is_percentage )
				percentage_width += row->cells[cell].width.size;
			else
				available_width -= row->cells[cell].width.size;
		}
		else
			unknown_cells++;
	}
	
	if ( percentage_width > 0 )
	{
		// rows with unknown size become min_cell_height
		int min_unknown_width = min_cell_width * unknown_cells;
		int total_percentage = 0;
		
		percentage_available = available_width;
		
		total_percentage = percentage_width;
		if ( total_percentage >= 100 )
			total_percentage = 100;
		
		percentage_available *= total_percentage / 100.0;
		
		if ( percentage_available > available_width - min_unknown_width )
			percentage_available = available_width - min_unknown_width;
		
		available_width = available_width - percentage_available;
	}
	else
	{
		percentage_width = 1;
	}
	
	if ( 1 )
	{
		float cell_shared_f = (float)available_width / (float)(unknown_cells<=0?1:unknown_cells);
		int cell_shared = floor(cell_shared_f);
		int percent_used = 0;
		int percent_space_used = 0;
		int x_pos = 0;
		
		for ( cell = 0; cell < row->numcells; cell++ )
		{
			lelex_cell_t *cellp = &row->cells[cell];
			int cell_width = 0;
			
			if ( cellp->width.is_set )
			{
				if ( cellp->width.is_percentage )
				{
					percent_used += cellp->width.size;
					cell_width = percentage_available * cellp->width.size / percentage_width;
					
					if ( percent_used == percentage_width )
					{
						cell_width = percentage_available - percent_space_used;
					}

					percent_space_used += cell_width;
				}
				else
				{
					cell_width = cellp->width.size;
				}
			}
			else
			{
				cell_width = cell_shared;
				unknown_cells--;
				
				if ( unknown_cells == 0 )
				{
					cell_width = ceil( cell_shared_f );
				}
			}
			
			row->cells[cell].bounds.x = x_pos;
			row->cells[cell].bounds.w = cell_width<=0?1:cell_width;
			
			//printf( " --> Cell %d (\"%s\") starts at %dpx and is %dpx wide.\n", cell, cellp->name, x_pos, cell_width );
			
			x_pos += cell_width;
		}
	}
}

void lelex_calculate( lelex_t *lel, int width, int height, int min_cell_width, int min_cell_height )
{
	int row, cell;
	int unknown_rows = 0;
	
	// the remaining height after statically heighted rows
	int available_height = height;
	
	// the total % of height requested from available
	int percentage_height = 0;
	int percentage_available = 0;
	
	// calculate how many unknown rows
	for ( row = 0; row < lel->numrows; row++ )
	{
		if ( lel->rows[row].height.is_set )
		{
			if ( lel->rows[row].height.is_percentage )
				percentage_height += lel->rows[row].height.size;
			else
				available_height -= lel->rows[row].height.size;
		}
		else
			unknown_rows++;
	}
	
	if ( percentage_height > 0 )
	{
		// rows with unknown size become min_cell_height
		int min_unknown_height = min_cell_height * unknown_rows;
		int total_percentage = 0;
		
		percentage_available = available_height;
		
		total_percentage = percentage_height;
		if ( total_percentage >= 100 )
			total_percentage = 100;
		
		percentage_available *= total_percentage / 100.0;
		
		if ( percentage_available > available_height - min_unknown_height )
			percentage_available = available_height - min_unknown_height;
		
		available_height = available_height - percentage_available;
	}
	else
	{
		percentage_height = 1;
	}
	
	if ( 1 )
	{
		float row_shared_f = (float)available_height / (float)(unknown_rows<=0?1:unknown_rows);
		int row_shared = floor(row_shared_f);
		int percent_used = 0;
		int percent_space_used = 0;
		int y_pos = 0;
		
		for ( row = 0; row < lel->numrows; row++ )
		{
			lelex_row_t *rowp = &lel->rows[row];
			int row_height = 0;
			
			if ( rowp->height.is_set )
			{
				if ( rowp->height.is_percentage )
				{
					percent_used += rowp->height.size;
					row_height = percentage_available * rowp->height.size / percentage_height;
					
					if ( percent_used == percentage_height )
					{
						row_height = percentage_available - percent_space_used;
					}

					percent_space_used += row_height;
				}
				else
				{
					row_height = rowp->height.size;
				}
			}
			else
			{
				row_height = row_shared;
				unknown_rows--;
				
				if ( unknown_rows == 0 )
				{
					row_height = ceil( row_shared_f );
				}
			}
			
			for ( cell = 0; cell < rowp->numcells; cell++ )
			{
				rowp->cells[cell].bounds.y = y_pos;
				rowp->cells[cell].bounds.h = row_height<=0?1:row_height;
			}
			
			//printf( "Row %d starts at %dpx and is %dpx high.\n", row, y_pos, row_height );
			
			y_pos += row_height;
			
			lelex_calculate_cells( lel, rowp, width, height, min_cell_width, min_cell_height );
		}
	}
}

lelex_cell_t *lelex_get_cell( lelex_t *lel, const char *name )
{
	int cell;
	
	for ( cell = 0; cell < lel->numcells; cell++ )
	{
		if ( !strcasecmp( lel->cells[cell].name, name ) )
			return &lel->cells[cell];
	}
	
	return NULL;
}
