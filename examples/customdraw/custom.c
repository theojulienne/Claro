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

object_t *w, *t, *b;

void push_my_button( object_t *obj, event_t *event )
{
	claro_shutdown( );
}

object_t *c;

#ifndef NO_CAIRO


void handle_main( object_t *obj, event_t *event )
{
    static int last_update = 0;
    int t;
	
    t = time( 0 );
	
    if ( t > last_update )
    {
        canvas_redraw( c );
        last_update = t;
    }
}

void handle_redraw( object_t *obj, event_t *event )
{
    /* this code was borrowed from cairoclock.tar.gz, can't remember
     * the site it came from: Google it for the original (gtkmm) source */
	
    //canvas_widget_t *cvsw = (canvas_widget_t *)obj;
    widget_t * cvsw = WIDGET(obj);    

    cairo_t *cr = (cairo_t*) event_get_ptr(event, "cairo_context"); //cvsw->cr;
	
    cairo_scale( cr, cvsw->size_req->w, cvsw->size_req->h );
	
    // store the current time
    time_t rawtime;
    time(&rawtime);
    struct tm * timeinfo = localtime (&rawtime);
	
    // compute the angles of the indicators of our clock
    double minutes = timeinfo->tm_min * M_PI / 30;
    double hours = timeinfo->tm_hour * M_PI / 6;
    double seconds= timeinfo->tm_sec * M_PI / 30;
	
    // who doesn't want all those nice line settings :)
    cairo_set_line_cap( cr, CAIRO_LINE_CAP_ROUND );
    cairo_set_line_width( cr, 0.1 );
	
    // translate to the center of the rendering context and draw a black clock outline
    cairo_set_source_rgba( cr, 0, 0, 0, 1 );
    cairo_translate( cr, 0.5, 0.5 );
    cairo_arc( cr, 0, 0, 0.4, 0, M_PI * 2 );
    cairo_stroke( cr );
	
    // draw a white dot on the current second.
    cairo_set_source_rgba( cr, 1, 1, 1, 0.6 );
    cairo_arc( cr, sin(seconds) * 0.4, -cos(seconds) * 0.4, 0.05, 0, M_PI * 2 );
    cairo_fill( cr );
	
    // draw the minutes wijzer
    cairo_set_source_rgba( cr, 0.2, 0.2, 1, 0.6 );
    cairo_move_to( cr, 0, 0 );
    cairo_line_to( cr, sin(minutes) * 0.4, -cos(minutes) * 0.4 );
    cairo_stroke( cr );

    // draw the hours wijzer		
    cairo_move_to( cr, 0, 0 );
    cairo_line_to( cr, sin(hours) * 0.2, -cos(hours) * 0.2 );
    cairo_stroke( cr );
}
#endif

int main( int argc, char *argv[] )
{
	claro_base_init( );
	claro_graphics_init( );
	
	log_fd_set_level( CL_DEBUG, stderr );
	
	clog( CL_INFO, "%s running using Claro!", __FILE__ );
	
	w = window_widget_create( 0, new_bounds( 100, 100, 230, 230 ), cWidgetCustomDraw );
	window_set_title( w, "Hello, World!" );
	
	/*
	t = textbox_widget_create( w, new_bounds( 10, 10, 210, -1 ), 0 );
	widget_set_notify( WIDGET(t), cNotifyKey );
	textbox_set_text( t, "Yeehaw!" );
	*/
	b = button_widget_create( w, new_bounds( 0, 0, 230, -1 ), 0 );
	button_set_text( b, "Close me!" );
	
	object_addhandler( b, "pushed", push_my_button );
	
#ifndef NO_CAIRO
	c = canvas_widget_create( w, new_bounds( 15, 30, 200, 200 ), 0 );
    object_addhandler(OBJECT(c), "redraw", handle_redraw );

	/* add our main loop */
    object_addhandler( claro, "mainloop", handle_main );
#endif
	
	window_show( w );
	window_focus( w );
	
	block_heap_loginfo( );
	
	claro_loop( );
	
	return 0;
}
