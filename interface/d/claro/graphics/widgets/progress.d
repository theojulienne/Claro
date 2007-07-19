module claro.graphics.widgets.progress;

import std.stdio;
import std.string;

private import claro.graphics.widget;

extern (C) {
	object_t *progress_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void progress_set_range( object_t *progress, double minimum, double maximum );
	void progress_set_level( object_t *progress, double value );
	void progress_set_orientation( object_t *progress, int style );
	double progress_get_value( object_t *progress );
}

enum cProgressOrientation {
	LeftRight = 0x0,
	RightLeft = 0x01,
	TopBottom = 0x02,
	BottomTop = 0x03,
}

class ProgressWidget : Widget {
	double _minimum = 0;
	double _maximum = 1;
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = progress_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void value( double d )
	{
		progress_set_level( this.obj, d );
	}
	
	void orientation( int s )
	{
		progress_set_orientation( this.obj, s );
	}
}
