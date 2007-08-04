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

#ifdef CLARO_OPENGL

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>

#include <X11/Xlib.h>
#include <gdk/gdkx.h>


static int get_glx_version()
{
    static int _glx_version = 0;

    if (_glx_version == 0)
    {
        // check the GLX version
        int major, minor;
        Bool ok = glXQueryVersion(GDK_DISPLAY(), &major, &minor);
        if (!ok)
            _glx_version = 10; // default to v1.0 
        else
            _glx_version = major * 10 + minor;
    }
    return _glx_version;
}

static void get_glx_attrib_list(int *gl_attribList )
{
    if (get_glx_version() >= 13)
        gl_attribList[0] = 0;
    else
    {
        int i = 0;
            // default settings if attriblist = 0
            gl_attribList[i++] = GLX_RGBA;
            gl_attribList[i++] = GLX_DOUBLEBUFFER;
            gl_attribList[i++] = GLX_DEPTH_SIZE;   gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_RED_SIZE;     gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_GREEN_SIZE;   gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_BLUE_SIZE;    gl_attribList[i++] = 1;
            gl_attribList[i++] = GLX_ALPHA_SIZE;   gl_attribList[i++] = 0;
            gl_attribList[i++] = None;
    }
}


#define GTK_TYPE_GL_AREA            (gtk_gl_area_get_type ())
#define GTK_GL_AREA(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_GL_AREA, GtkGLArea))
#define GTK_GL_AREA_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_GL_AREA, GtkGLAreaClass))
#define GTK_IS_GL_AREA(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_GL_AREA))
#define GTK_IS_GL_AREA_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_GL_AREA))
#define GTK_GL_AREA_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_GL_AREA, GtkGLAreaClass))


typedef struct _GtkGLArea       GtkGLArea;
typedef struct _GtkGLAreaClass  GtkGLAreaClass;

struct _GtkGLArea
{
    GtkWidget widget;
    GdkVisual * gdk_visual;
    GdkColormap * gdk_colormap;
    GLXContext glx_context;
    XVisualInfo * glx_visual;
};

struct _GtkGLAreaClass
{
  GtkWidgetClass parent_class;
};


GType      gtk_gl_area_get_type   (void) G_GNUC_CONST;
GtkWidget* gtk_gl_area_new        (void);

/*
void       gtk_drawing_area_size       (GtkDrawingArea      *darea,
					gint                 width,
					gint                 height);
*/

static void gtk_gl_area_realize       (GtkWidget           *widget);
static void gtk_gl_area_size_allocate (GtkWidget           *widget, GtkAllocation       *allocation);

static void gtk_gl_area_send_configure (GtkGLArea  * gl_area);

G_DEFINE_TYPE (GtkGLArea, gtk_gl_area, GTK_TYPE_WIDGET)

static void
gtk_gl_area_class_init (GtkGLAreaClass *class)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  widget_class->realize = gtk_gl_area_realize;
  widget_class->size_allocate = gtk_gl_area_size_allocate; 
}

//static int _glx_attr_list[] = { 5, GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1, GLX_DEPTH_SIZE, 1, 0  }; 

static void
gtk_gl_area_init (GtkGLArea * gl_area)
{  
    Display * dpy;
    int attribList[512];

    dpy = GDK_DISPLAY();

    get_glx_attrib_list( attribList );
     
    gl_area->glx_visual = glXChooseVisual(dpy, DefaultScreen(dpy), attribList); //_glx_attr_list);
  
    gl_area->gdk_visual = gdkx_visual_get(gl_area->glx_visual->visualid);
    gl_area->gdk_colormap = gdk_colormap_new(gl_area->gdk_visual, TRUE);

    gtk_widget_set_double_buffered(GTK_WIDGET(gl_area), FALSE);
}


GtkWidget*
gtk_gl_area_new (void)
{
  return g_object_new (GTK_TYPE_GL_AREA, NULL);
}

/*
void
gtk_drawing_area_size (GtkDrawingArea *darea,
		       gint            width,
		       gint            height)
{
  g_return_if_fail (GTK_IS_GL_AREA (darea));

  GTK_WIDGET (darea)->requisition.width = width;
  GTK_WIDGET (darea)->requisition.height = height;

  gtk_widget_queue_resize (GTK_WIDGET (darea));
}
*/

static void
gtk_gl_area_realize (GtkWidget * widget)
{
  GtkGLArea * gl_area;
  GdkWindowAttr attributes;
  gint attributes_mask;

  g_return_if_fail (GTK_IS_GL_AREA (widget));

  gl_area = GTK_GL_AREA (widget);
  GTK_WIDGET_SET_FLAGS (widget, GTK_REALIZED);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = widget->allocation.x;
  attributes.y = widget->allocation.y;
  attributes.width = widget->allocation.width;
  attributes.height = widget->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  //attributes.visual = gl_area->gdk_visual;
  //attributes.colormap = gl_area->gdk_colormap;
  attributes.event_mask = gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y; // | GDK_WA_COLORMAP | GDK_WA_VISUAL;

  gtk_widget_push_colormap( gl_area->gdk_colormap );

  widget->window = gdk_window_new (gtk_widget_get_parent_window (widget), &attributes, attributes_mask);
  gdk_window_set_user_data (widget->window, gl_area);

  gtk_widget_pop_colormap();

  widget->style = gtk_style_attach (widget->style, widget->window);
  gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);

  //gdk_window_set_back_pixmap(widget->window, NULL, FALSE); 
  gl_area->glx_context = glXCreateContext(GDK_DISPLAY(), gl_area->glx_visual, NULL, TRUE);

  gtk_gl_area_send_configure (GTK_GL_AREA (widget));
}

static void
gtk_gl_area_size_allocate (GtkWidget     *widget,
				GtkAllocation *allocation)
{
  g_return_if_fail (GTK_IS_GL_AREA (widget));
  g_return_if_fail (allocation != NULL);

  widget->allocation = *allocation;

  if (GTK_WIDGET_REALIZED (widget))
    {
      gdk_window_move_resize (widget->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      gtk_gl_area_send_configure (GTK_GL_AREA (widget));
    }
}

static void
gtk_gl_area_send_configure (GtkGLArea * gl_area)
{
  GtkWidget *widget;
  GdkEvent *event = gdk_event_new (GDK_CONFIGURE);

  widget = GTK_WIDGET (gl_area);

  event->configure.window = g_object_ref (widget->window);
  event->configure.send_event = TRUE;
  event->configure.x = widget->allocation.x;
  event->configure.y = widget->allocation.y;
  event->configure.width = widget->allocation.width;
  event->configure.height = widget->allocation.height;
  
  gtk_widget_event (widget, event);
  gdk_event_free (event);
}

static void gtk_gl_area_activate(GtkWidget * widget)
{    
    if(GTK_WIDGET_REALIZED(widget))
        glXMakeCurrent(GDK_DISPLAY(), GDK_WINDOW_XID(widget->window), GTK_GL_AREA(widget)->glx_context);
}

static void gtk_gl_area_flip(GtkWidget * widget)
{
    if(GTK_WIDGET_REALIZED(widget))
        glXSwapBuffers(GDK_DISPLAY(), GDK_WINDOW_XID(widget->window)); 
}

static gint cgraphics_opengl_mouse_move( GtkWidget *widget, GdkEventMotion *event, widget_t *w )
{
	int x, y;
	GdkModifierType state;
	
	if ( event->is_hint )
		gdk_window_get_pointer (event->window, &x, &y, &state);
	else
	{
		x = event->x;
		y = event->y;
		state = event->state;
	}

//  this doesnt seem to be used anymore..
//  if ( w->notify_flags & cNotifyMouse )
	event_send( OBJECT(w), "mouse_moved", "ii", "x", x, "y", y );
	
	return TRUE;
}

static gint cgraphics_opengl_mouse_down( GtkWidget *widget, GdkEventButton *event, widget_t *w )
{
	int x, y;
	
	x = (int)event->x;
	y = (int)event->y;
	
	switch ( event->button )
	{
		case 1:
			event_send( OBJECT(w), "clicked", "ii", "x", x, "y", y );
			break;
		case 3:
			event_send( OBJECT(w), "right_clicked", "ii", "x", x, "y", y );
			break;
	}
	
	return FALSE;
}

static gint cgraphics_opengl_mouse_up( GtkWidget *widget, GdkEventButton *event, widget_t * w)
{
	int x, y;
	
	x = (int)event->x;
	y = (int)event->y;
	
	switch ( event->button )
	{
		case 1:
			event_send( OBJECT(w), "released", "ii", "x", x, "y", y );
			break;
		case 3:
			event_send( OBJECT(w), "right_released", "ii", "x", x, "y", y );
			break;
	}
	
	return FALSE;
}

void cgraphics_opengl_widget_create(widget_t * widget)
{
	widget->native = (void*) gtk_gl_area_new();
	
	gtk_widget_set_events(GTK_WIDGET(widget->native), GDK_POINTER_MOTION_MASK | GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | GDK_POINTER_MOTION_HINT_MASK);
	
	g_signal_connect(G_OBJECT(widget->native), "motion-notify-event", G_CALLBACK(cgraphics_opengl_mouse_move), widget);
	g_signal_connect(G_OBJECT(widget->native), "button-press-event", G_CALLBACK(cgraphics_opengl_mouse_down), widget);
	g_signal_connect(G_OBJECT(widget->native), "button-release-event", G_CALLBACK(cgraphics_opengl_mouse_up), widget);
	
	cgraphics_widget_create(widget);
}

void cgraphics_opengl_flip(widget_t * widget)
{
    if(G_IS_OBJECT(widget->native))
        gtk_gl_area_flip(GTK_WIDGET(widget->native));
}

void cgraphics_opengl_activate(widget_t * widget)
{
    if(G_IS_OBJECT(widget->native))
        gtk_gl_area_activate(GTK_WIDGET(widget->native));
}

#endif
