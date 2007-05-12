module claro.graphics.widgets.button;

import std.stdio;
import std.string;

private import claro.graphics.widget;

extern (C) {
	object_t *button_widget_create( object_t *parent, bounds_t *bounds, int flags );
	object_t *button_widget_create_with_label( object_t *parent, bounds_t *bounds, int flags, char *label );
	void button_set_label( object_t *obj, char *label );
}

class ButtonWidget : Widget {
	this( CObject parent, Bounds b, int flags, char[] label="" ) {
		super( parent );
		
		this.b = b;
		this.obj = button_widget_create_with_label( CObject.getObj(parent), this.b.obj, flags, std.string.toStringz(label) );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void label( char[] label ) {
		button_set_label( this.obj, std.string.toStringz(label) );
	}
}
