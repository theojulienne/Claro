module claro.graphics.widgets.menu;

private import claro.graphics.widget;
private import claro.graphics.listitem;
private import claro.graphics.image;

extern (C)
{
	object_t *menu_widget_create( object_t *parent, int flags );
	list_item_t *menu_append_item( object_t *menu, list_item_t *parent, image_t *image, char *title );
	void menu_popup( object_t *menu, int x, int y, int flags );
}

class MenuWidget : Widget {
	static PopupAtCursor = 1;
	
	this( CObject parent, int flags ) {
		super( parent );
		
		this.obj = menu_widget_create( CObject.getObj(parent), flags );
		this.created( );
	}
	
	this( CObject parent ) {
		this( parent, 0 );
	}
	
	ListItem appendItem( ListItem parent, Image im, char[] title ) {
		auto obj = menu_append_item( this.obj, cast(list_item_t*) CObject.getObj(parent), cast(image_t*) CObject.getObj(im), std.string.toStringz(title) );
		return new ListItem( obj, parent );
	}
	
	ListItem appendItem( ListItem parent, char[] title ) {
		return this.appendItem( parent, null, title );
	}
	
	ListItem appendItem( char[] title ) {
		return this.appendItem( null, null, title );
	}
	
	ListItem appendItem( Image im, char[] title ) {
		return this.appendItem( null, im, title );
	}
	
	void popup( int x, int y, int flags )
	{
		menu_popup( this.obj, x, y, flags );
	}
	
	void popup( int flags )
	{
		popup( 0, 0, flags );
	}
	
	void popup( )
	{
		popup( PopupAtCursor );
	}
}