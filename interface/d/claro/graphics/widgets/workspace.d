module claro.graphics.widgets.workspace;

import std.stdio;

private import claro.graphics.widget;
private import claro.graphics.image;

extern (C) {
	object_t *workspace_widget_create( object_t *parent, bounds_t *bounds, int flags );
	
	void workspace_set_active( object_t *workspace, object_t *child );
	object_t *workspace_get_active( object_t *workspace );
	
	void workspace_cascade( object_t *workspace );
	void workspace_tile( object_t *workspace, int dir );
	
	object_t *workspace_window_widget_create( object_t *parent, bounds_t *bounds, int flags );
	
	void workspace_window_set_title( object_t *window, char *title );
	void workspace_window_set_icon( object_t *w, image_t *icon );
	
	void workspace_window_show( object_t *window );
	void workspace_window_hide( object_t *window );
	void workspace_window_restore( object_t *window );
	void workspace_window_minimise( object_t *window );
	void workspace_window_maximise( object_t *window );
}

class WorkspaceWidget : Widget {
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = workspace_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void active( WorkspaceWindow win )
	{
		workspace_set_active( this.obj, win.obj );
	}
	
	WorkspaceWindow active( )
	{
		return null;
	}
	
	void tile( int dir = 0 )
	{
		workspace_tile( this.obj, dir );
	}
	
	void cascade( )
	{
		workspace_cascade( this.obj );
	}
}

class WorkspaceWindow : Widget
{
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = workspace_window_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void show( ) {
		workspace_window_show( this.obj );
	}
	
	void focus( ) {
		//window_focus( this.obj );
	}
	
	void title( char[] title ) {
		workspace_window_set_title( this.obj, std.string.toStringz(title) );
	}
	
	// FIXME: return the window's title
	char[] title( ) { return ""; }
	
	void icon( Image icon )
	{
		if ( icon is null )
			workspace_window_set_icon( this.obj, null );
		
		workspace_window_set_icon( this.obj, icon.obj );
	}
}
