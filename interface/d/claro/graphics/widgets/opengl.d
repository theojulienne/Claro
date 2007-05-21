module claro.graphics.widgets.opengl;

import std.stdio;

public import claro.graphics.cairooo.all;
private import claro.graphics.widget;
private import claro.graphics.cairo.cairotypes;

extern (C) {
	object_t *opengl_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void opengl_flip( object_t *widget );
	void opengl_activate( object_t *widget );
}

class OpenGLWidget : Widget {
	void delegate(OpenGLWidget w) redrawDelegate = null;
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = opengl_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
		
		this.add_handler( "redraw", &this.internalRedrawDelegate );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void activate( ) {
		opengl_activate( this.obj );
	}
	
	void flip( ) {
		opengl_flip( this.obj );
	}
	
	void redraw( ) {
		if ( this.redrawDelegate is null )
			return;
		this.redrawDelegate( this );
	}
	
	void internalRedrawDelegate( CEvent evt, CObject obj ) {
		this.activate( );
		this.redraw( );
	}
	
	void redraw_delegate( void delegate(OpenGLWidget w) d ) {
		redrawDelegate = d;
	}
	
	void delegate(OpenGLWidget w) redraw_delegate( ) {
		return redrawDelegate;
	}
}
