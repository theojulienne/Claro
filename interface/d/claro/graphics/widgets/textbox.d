module claro.graphics.widgets.textbox;

import std.stdio;
import std.string;

private import claro.graphics.widget;

extern (C) {
	object_t *textbox_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void textbox_set_text( object_t *obj, char *text );
	char *textbox_get_text( object_t *obj );
}

class TextBoxWidget : Widget {
	this( CObject parent, Bounds b, int flags, char[] text="" ) {
		super( parent );
		
		this.b = b;
		this.obj = textbox_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
		
		this.text = text;
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void text( char[] text ) {
		textbox_set_text( this.obj, std.string.toStringz(text) );
	}
	
	char[] text( ) {
		char *foo = textbox_get_text( this.obj );
		return std.string.toString(foo);
	}
}
