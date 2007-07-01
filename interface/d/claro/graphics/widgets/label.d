module claro.graphics.widgets.label;

import std.stdio;
import std.string;

private import claro.graphics.widget;

extern (C) {
	object_t *label_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void label_set_text( object_t *obj, char *text );
	void label_set_justify( object_t *obj, int flags );
}

class LabelWidget : Widget {
	enum Justify {
		Left = 0x01,
		Right = 0x02,
		Center = 0x04,
		Fill = 0x08
	}
	
	this( CObject parent, Bounds b, int flags, char[] text="" ) {
		super( parent );
		
		this.b = b;
		this.obj = label_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
		
		this.text = text;
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void text( char[] text ) {
		label_set_text( this.obj, std.string.toStringz(text) );
	}
	
	void justify( Justify justification ) {
		label_set_justify( this.obj, cast(int)justification );
	}
}
