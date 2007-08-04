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
#include <claro/graphics.h>

// the original (un-encoded) Unicode sequence is located in i18n_original.txt
#define UTF8_TEXT	"\x20\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e\x3a\x20\xe7\x8c\xbf\xe3\x82\x82\xe6\x9c\xa8\xe3" \
					"\x81\x8b\xe3\x82\x89\xe8\x90\xbd\xe3\x81\xa1\xe3\x82\x8b\xe3\x80\x82\n\n" \
					"\x20\xe4\xb8\xad\xe6\x96\x87\xef\xbc\x9a\n" \
					"\x20\xe5\xb1\xb1\xe4\xb8\xad\xe9\x97\xae\xe7\xad\x94\n\n" \
					"\x20\xe9\x97\xae\xe4\xbd\x99\xe4\xbd\x95\xe6\x84\x8f\xe6\xa0\x96\xe7\xa2\xa7\xe5\xb1\xb1\n" \
					"\x20\xe7\xac\x91\xe8\x80\x8c\xe4\xb8\x8d\xe5\xa4\xa7\xe5\xbf\x83\xe8\x87\xaa\xe9\x97\xb2\n" \
					"\x20\xe6\xa1\x83\xe8\x8a\xb1\xe6\xb5\x81\xe6\xb0\xb4\xe7\xaa\x85\xe7\x84\xb6\xe5\x8e\xbb\n" \
					"\x20\xe5\x88\xab\xe6\x9c\x89\xe5\xa4\xa9\xe5\x9c\xb0\xe5\x9c\xa8\xe4\xba\xba\xe9\x97\xb4\n\n" \
					"\x20\xe6\x9d\x8e\xe7\x99\xbd\n\n"

object_t *w, *t;

static void window_closed(object_t *window, event_t *event) 
{
    claro_shutdown();
}

int main( int argc, char *argv[] )
{
	claro_base_init( );
	claro_graphics_init( );

	log_fd_set_level( CL_DEBUG, stderr );

	clog( CL_INFO, "%s running using Claro!", __FILE__ );

	w = window_widget_create( 0, new_bounds( 100, 100, 230, 230 ), 0 );
	window_set_title( w, "claro i18n test" );
	object_addhandler(w, "destroy", window_closed);
	
	t = label_widget_create_with_text(w, new_bounds(0, 0, 230, 230), 0, UTF8_TEXT);

	window_show( w );
	window_focus( w );

	block_heap_loginfo( );

	claro_loop( );

	return 0;
}


