module claro.graphics.widgets.window;

private import claro.graphics.widget;
private import claro.graphics.image;

extern (C)
{
	object_t *window_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void window_show( object_t * );
	void window_focus( object_t * );
	void window_set_title( object_t*, char *title );
	void window_set_icon( object_t*, object_t* );
}

class WindowWidget : Widget {
	static const int ModalDialog = 1;
	static const int CenterParent = 2;
	static const int NoResizing = 4;
	static const int ToolStyle = 8;
	static const int Floating = 16;
	
	this()
	{
	    super();
	}
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = window_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void show( ) {
		window_show( this.obj );
	}
	
	void focus( ) {
		window_focus( this.obj );
	}
	
	void title( char[] title ) {
		window_set_title( this.obj, std.string.toStringz(title) );
	}
	
	// FIXME: return the window's title
	char[] title( ) { return ""; }
	
	void icon( Image icon )
	{
		if ( icon is null )
			window_set_icon( this.obj, null );
		
		window_set_icon( this.obj, icon.obj );
	}
}
