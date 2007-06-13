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

//gint cgraphics_button_clicked_handler( GtkWidget *widget, widget_t *w );

static void _on_tab_closed(GtkButton * button, widget_t * tab)
{
    widget_close(OBJECT(tab));
}

static GtkWidget * _create_tab_label(GtkNotebook * notebook, widget_t * tab)
{
    GtkWidget *hbox, *label_hbox, *label_ebox;
	GtkWidget *label;
	GtkWidget *close_button;
	GtkSettings *settings;
	gint w, h;
	GtkWidget *image;
	GtkWidget *icon;

	hbox = gtk_hbox_new (FALSE, 0);

	label_ebox = gtk_event_box_new ();
	gtk_event_box_set_visible_window (GTK_EVENT_BOX (label_ebox), FALSE);
	gtk_box_pack_start (GTK_BOX (hbox), label_ebox, TRUE, TRUE, 0);

	label_hbox = gtk_hbox_new (FALSE, 0);
	gtk_container_add (GTK_CONTAINER (label_ebox), label_hbox);

	close_button = gtk_button_new ();
	gtk_button_set_relief (GTK_BUTTON (close_button), GTK_RELIEF_NONE);
	gtk_button_set_focus_on_click (GTK_BUTTON (close_button), FALSE);

	settings = gtk_widget_get_settings (GTK_WIDGET (notebook));
	gtk_icon_size_lookup_for_settings (settings, GTK_ICON_SIZE_MENU, &w, &h);
	image = gtk_image_new_from_stock (GTK_STOCK_CLOSE, GTK_ICON_SIZE_MENU);
	//gtk_widget_set_size_request (close_button, w + 2, h + 2);
	gtk_container_add (GTK_CONTAINER (close_button), image);
	gtk_box_pack_start (GTK_BOX (hbox), close_button, FALSE, FALSE, 0);

	g_signal_connect (G_OBJECT (close_button), "clicked",
                          G_CALLBACK (_on_tab_closed),
                          tab);


	icon = gtk_image_new ();
	gtk_box_pack_start (GTK_BOX (label_hbox), icon, FALSE, FALSE, 2);

    label = gtk_label_new ("");
	gtk_misc_set_alignment (GTK_MISC (label), 0.0, 0.5);
    gtk_misc_set_padding (GTK_MISC (label), 2, 0);
	gtk_box_pack_start (GTK_BOX (label_hbox), label, FALSE, FALSE, 0);
	
	gtk_widget_show (hbox);
	gtk_widget_show (label_ebox);
	gtk_widget_show (label_hbox);
	gtk_widget_show (label);
	gtk_widget_show (image);
	gtk_widget_show (close_button);
	gtk_widget_show (icon);
	
	g_object_set_data (G_OBJECT (hbox), "label", label);
	g_object_set_data (G_OBJECT (hbox), "icon", icon);
	g_object_set_data (G_OBJECT (hbox), "close-button", close_button);
    
	return hbox; 
}


/* called at initialisation */
void cgraphics_init( )
{
	gtk_init( NULL, NULL );
}

/* called each loop, use to process events from the platform */
void cgraphics_check_events( )
{
	gtk_main_iteration_do( FALSE );
}

/* resize handler */
static gint cgraphics_resized_handler( GtkWidget *widget, GdkEventConfigure *event, object_t *w )
{
	widget_set_size( w, event->width, event->height, 1 );
	widget_set_content_size( w, event->width, event->height, 1 );
	widget_set_position( w, event->x, event->y, 1 );
	
	return 0;
}

void _cgraphics_screen_changed_handler( GtkWidget *widget, GdkScreen *arg1, widget_t *w )
{
	GdkScreen *sc;
	GdkColormap *cm;
	
	gtk_widget_realize( widget );
	
	sc = gtk_widget_get_screen( widget );
	
	cm = gdk_screen_get_rgba_colormap( sc );
	
	w->supports_alpha = 0;
	
	if ( cm == NULL )
	{
		printf( "Warning: Alpha not supported\n" );
		cm = gdk_screen_get_rgb_colormap( sc );
	}
	else
		w->supports_alpha = 1;
	
	gtk_widget_set_colormap( widget, cm );
}

gboolean _cgraphics_cd_expose_handler( GtkWidget *widget, GdkEventExpose *event, widget_t *w )
{
	cairo_t *cr;
	
	cr = gdk_cairo_create( GDK_DRAWABLE(widget->window) );
	
	cairo_set_source_rgba( cr, 1.0, 0.0, 0.0, 1.0 );
	cairo_set_operator( cr, CAIRO_OPERATOR_SOURCE );
	cairo_paint( cr );
	
	return FALSE;
}

gint cgraphics_resized2_handler( GtkWidget *widget, GtkAllocation *event, widget_t *w )
{
	if ( object_is_of_class( OBJECT(w), "container_widget" ) ||
		 object_is_of_class( OBJECT(w), "frame_widget" ) ||
		 object_is_of_class( OBJECT(w), "workspace_window_widget" ) ||
		 object_is_of_class( OBJECT(w), "window_widget" )	)
	{
		if ( event->x == w->size.x && event->y == w->size.y &&
			 event->width == w->size.w && event->height == w->size.h )
			return 0;
		
		widget_set_size( OBJECT(w), event->width, event->height, 1 );
		widget_set_content_size( OBJECT(w), event->width, event->height, 1 );
		widget_set_position( OBJECT(w), event->x, event->y, 1 );
		
		widget_resized_handle( OBJECT(w), 0 );
	}
	
	return 0;
}

/* destroy handler */
gint cgraphics_destroy_handler( widget_t *w )
{
	widget_destroy( OBJECT(w) );
 
	return 1;
}

/* General native widget helpers */

void cgraphics_widget_create( widget_t *widget )
{
	widget_t *parent;
	PangoFontDescription * font_desc;

	parent = (widget_t *)object_parent(widget);
	
	/* check for parent */
	if ( parent == NULL )
	{
		clog( CL_ERROR, "Widget requires parent, none given." );
		return;
	}
	
	font_desc = ((GtkWidget*)widget->native)->style->font_desc;
	//printf("%s: FONT_SIZE %d\n", __FUNCTION__, pango_font_description_get_size(font_desc));
		
	/* FIXME: height should be determined from font height in pixels */
	//if ( widget->size_req->h == -1 )
	//	widget->size_req->h = 25;
	
	gtk_widget_set_size_request( GTK_WIDGET(widget->native), widget->size_req->w, widget->size_req->h );
	
	g_signal_connect( G_OBJECT(widget->native), "configure_event", G_CALLBACK(cgraphics_resized_handler), widget );
	g_signal_connect_swapped( G_OBJECT(widget->native), "destroy", G_CALLBACK(cgraphics_destroy_handler), widget );
	
	if ( widget->flags & cWidgetCustomDraw )
	{
		g_signal_connect( G_OBJECT(widget->native), "screen-changed", G_CALLBACK(_cgraphics_screen_changed_handler), widget );
		g_signal_connect( G_OBJECT(widget->native), "expose-event", G_CALLBACK(_cgraphics_cd_expose_handler), widget );
	}
	
	if ( widget->container != 0 )
		g_signal_connect( G_OBJECT(widget->container), "size_allocate", G_CALLBACK(cgraphics_resized2_handler), widget );
	else
		g_signal_connect( G_OBJECT(widget->native), "size_allocate", G_CALLBACK(cgraphics_resized2_handler), widget );
	
	if ( object_is_of_class( OBJECT(parent), "splitter_widget" ) )
	{
		int splitter_num = (int)parent->ndata;
		splitter_widget_t *sw = (splitter_widget_t *)parent;
			
		if ( splitter_num == 0 )
			gtk_paned_pack1( GTK_PANED(parent->native), widget->native, true, false );
		else
			gtk_paned_pack2( GTK_PANED(parent->native), widget->native, true, false );
		
		sw->children[splitter_num].w = widget;
		
		parent->ndata = (void *)(splitter_num+1);
	}
	else if ( object_is_of_class( OBJECT(parent), "workspace_widget" ) )
	{
	    printf("%s: creating tab widget..\n", __FUNCTION__);
	    GtkNotebook * nb = GTK_NOTEBOOK(widget_get_container(OBJECT(parent)));
	    GtkWidget * label = _create_tab_label(nb, widget);
		gtk_notebook_append_page(nb, GTK_WIDGET(widget->native), label);
	}
	else
	{
		gtk_layout_put( GTK_LAYOUT(widget_get_container(OBJECT(parent))), widget->native, widget->size_req->x, widget->size_req->y );
	}
	
	g_object_set_data( widget->native, "claro_widget", widget );
	
	if ( widget->flags & cWidgetCustomDraw )
		_cgraphics_screen_changed_handler( widget->native, NULL, widget );
	
	gtk_widget_show( GTK_WIDGET(widget->native) );
}

widget_t *cgraphics_gtk_to_claro_widget( GtkWidget *w )
{
	widget_t *objval;
	objval = g_object_get_data( G_OBJECT(w), "claro_widget" );
	return objval;
}

/* General widgets */

void cgraphics_widget_show( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_show( GTK_WIDGET(widget->native) );
}

void cgraphics_widget_hide( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_hide( GTK_WIDGET(widget->native) );
}

void cgraphics_widget_focus( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_grab_focus( GTK_WIDGET(widget->native) );
}

void cgraphics_post_init( widget_t *widget )
{
	
}

void cgraphics_widget_enable( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_set_sensitive( GTK_WIDGET(widget->native), TRUE );
}

void cgraphics_widget_disable( widget_t *widget )
{
	if ( widget->native == 0 )
		return;
	
	gtk_widget_set_sensitive( GTK_WIDGET(widget->native), FALSE );
}

void cgraphics_widget_close( widget_t *widget )
{
	if ( widget->container != NULL )
		gtk_widget_destroy( widget->container );
	
	gtk_widget_destroy( widget->native );
}

widget_t *cgraphics_get_widget_window( widget_t *w )
{
	object_t *o ;
	
	o = (object_t *)w;
	
	if ( object_is_of_class( OBJECT(o), "window_widget" ) )
		return w;
	
	if ( o->parent == 0 )
		return 0;
	
	return cgraphics_get_widget_window( (widget_t *)o->parent );
}

/* update bounds (global widget function */

void cgraphics_update_bounds( object_t *obj )
{
	widget_t *widget = (widget_t *)obj;

	if (widget->size_req == NULL)
	{
		// these widgets use special OS-determined bounds.
		return;
	}
	
	if ( widget->native == NULL )
		return;

	if ( widget->size_req->w <= -1 || widget->size_req->h <= -1 )
		clog( CL_ERROR, "widget '%s' @ %p about to be sized with negative width or height!", obj->type, obj );
	
	gtk_widget_set_size_request( widget->native, widget->size_req->w, widget->size_req->h );
	
	if ( !object_is_of_class( OBJECT(obj->parent), "splitter_widget" ) )
		gtk_layout_move( GTK_LAYOUT(GTK_WIDGET(widget->native)->parent), widget->native, widget->size_req->x, widget->size_req->y );
}
/* "borrowed" from http://mail.gnome.org/archives/gtk-app-devel-list/2004-November/msg00028.html */
static void widget_get_rect_in_screen( GtkWidget *widget, GdkRectangle *r )
{
	gint x,y,w,h;
	GdkRectangle extents;
	GdkWindow *window;
	window = gtk_widget_get_parent_window(widget); /* getting parent window */
	gdk_window_get_root_origin(window, &x,&y); /* parent's left-top screen coordinates */
	gdk_drawable_get_size(window, &w,&h); /* parent's width and height */
	gdk_window_get_frame_extents(window, &extents); /* parent's extents (including decorations) */
	r->x = x + (extents.width-w)/2 + widget->allocation.x; /* calculating x (assuming: left border size == right border size) */
	r->y = y + (extents.height-h)-(extents.width-w)/2 + widget->allocation.y; /* calculating y (assuming: left border size == right border size == bottom border size) */
	r->width = widget->allocation.width;
	r->height = widget->allocation.height;
}

void cgraphics_widget_screen_offset( widget_t *object, int *dx, int *dy )
{
	GdkRectangle r;
	widget_get_rect_in_screen( object->native, &r );
	*dx = r.x;
	*dy = r.y;
}

GdkCursorType cgraphics_cursors[] = {
	GDK_LEFT_PTR, //cCursorNormal=0,
	GDK_XTERM, //cCursorTextEdit,
	GDK_WATCH, //cCursorWait,
	GDK_HAND2, //cCursorPoint,
};

void cgraphics_widget_set_cursor( widget_t *widget, int cursor )
{
	GdkCursor *gcursor;
	
	gcursor = gdk_cursor_new( cgraphics_cursors[cursor] );
	gdk_window_set_cursor( GDK_WINDOW(GTK_WIDGET(widget->native)->window), gcursor );
	gdk_cursor_unref( gcursor );
}


void cgraphics_cursor_capture( )
{
	
}

void cgraphics_cursor_release( )
{
	
}

void cgraphics_cursor_hide( )
{
	
}

void cgraphics_cursor_show( )
{
	
}

/* end of platform functions */
