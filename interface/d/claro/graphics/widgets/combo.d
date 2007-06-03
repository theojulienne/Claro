module claro.graphics.widgets.combo;

private import claro.graphics.widget;
private import claro.graphics.listitem;
private import claro.graphics.image;

extern (C)
{
	object_t *combo_widget_create( object_t *parent, bounds_t *bounds, int flags );
	list_item_t *combo_append_row( object_t *menubar, char *title );
	void combo_select_item( object_t *obj, list_item_t *item );
	list_item_t *combo_get_selected( object_t *obj );
}

class ComboWidget : Widget {
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = combo_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	ListItem appendItem( char[] title ) {
		auto obj = combo_append_row( this.obj, std.string.toStringz(title) );
		return new ListItem( obj, null );
	}
	
	void selected( ListItem item )
	{
		combo_select_item( this.obj, cast(list_item_t*)item.obj );
	}
	
	ListItem selected( )
	{
		return cast(ListItem)CObject.forObject( cast(object_t*)combo_get_selected( this.obj ) );
	}
}