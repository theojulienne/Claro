module claro.graphics.widgets.splitter;

import std.stdio;

private import claro.graphics.widget;

extern (C) {
	object_t *splitter_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void splitter_set_info( object_t *splitter, int child, int flex, int size );
}

class SplitterWidget : Widget {
	static const int Horizontal = 0;
	static const int Vertical = 1;
	
	static const int First = 0;
	static const int Second = 1;
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = splitter_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void setInfo( int child, bool flex, int size )
	{
		splitter_set_info( this.obj, child, flex, size );
	}
}
