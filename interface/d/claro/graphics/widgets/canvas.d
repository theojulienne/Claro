module claro.graphics.widgets.canvas;

import std.stdio;

public import claro.graphics.cairooo.all;
private import claro.graphics.widget;
private import claro.graphics.cairo.cairotypes;

extern (C) {
	object_t *canvas_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void canvas_redraw( object_t *widget );
	cairo_t *canvas_get_cairo_context( object_t *widget );
}

class CanvasWidget : Widget {
	void delegate(CanvasWidget w, Context c) redrawDelegate = null;
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = canvas_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
		
		this.add_handler( "redraw", &this.internalRedrawDelegate );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void redraw( ) {
		canvas_redraw( this.obj );
	}
	
	void internalRedrawDelegate( CEvent evt, CObject obj ) {
		Context c = new Context( canvas_get_cairo_context(this.obj), true );
		if ( this.redrawDelegate is null )
			return;
		this.redrawDelegate( this, c );
	}
	
	void redraw_delegate( void delegate(CanvasWidget w, Context c) d ) {
		redrawDelegate = d;
	}
	
	void delegate(CanvasWidget w, Context c) redraw_delegate( ) {
		return redrawDelegate;
	}
}
