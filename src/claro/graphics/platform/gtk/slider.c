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
#include <assert.h>
#include <claro/graphics/platform.h>


static void cgraphics_slider_changed_handler(GtkRange * range, widget_t * w)
{
    event_send( OBJECT(w), "changed", "" );
}

void cgraphics_slider_widget_create( widget_t *widget )
{
    if(widget->flags & cSliderVertical)
        widget->native = (void *)gtk_vscale_new_with_range(0.0, 1.0, 0.1);
    else
    {
        widget->native = (void *)gtk_hscale_new_with_range(0.0, 1.0, 0.1);
    }	

    cgraphics_widget_create( widget );
	
	g_signal_connect( G_OBJECT(widget->native), "value-changed", 
        G_CALLBACK(cgraphics_slider_changed_handler), widget );
}

void cgraphics_slider_set_value( widget_t *progress, double value )
{
	gtk_range_set_value(GTK_RANGE(progress->native), value);
}

void cgraphics_slider_set_range( widget_t *progress, double minimum, double maximum )
{
	gtk_range_set_range(GTK_RANGE(progress->native), minimum, maximum);
}


//probably not very realistic to allow change of the style after creation..	
void cgraphics_slider_set_style( widget_t *progress, int style )
{
}

double cgraphics_slider_get_value( widget_t *slider )
{
    return gtk_range_get_value(GTK_RANGE(slider->native));	
}


