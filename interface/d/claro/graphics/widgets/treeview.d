module claro.graphics.widgets.treeview;

import std.stdio;

private import claro.graphics.widget;
private import claro.graphics.listitem;
private import claro.graphics.image;

extern (C) {
	object_t *treeview_widget_create( object_t *parent, bounds_t *bounds, int flags );
	
	list_item_t *treeview_append_row( object_t *treeview, list_item_t *parent, image_t *image, char *title );
	list_item_t *treeview_insert_row( object_t *treeview, list_item_t *parent, int pos, image_t *image, char *title );
	void treeview_move_row( object_t *treeview, list_item_t *item, int row );
	void treeview_remove_row( object_t *treeview, list_item_t *item );
	void treeview_expand( object_t *treeview, list_item_t *item );
	void treeview_collapse( object_t *treeview, list_item_t *item );
	list_item_t *treeview_get_selected( object_t *obj );
	int treeview_get_rows( object_t *obj, list_item_t *parent );
	void treeview_select_item( object_t *obj, list_item_t *item );
}

class TreeViewWidget : Widget {
	static const int Horizontal = 0;
	static const int Vertical = 1;
	
	static const int First = 0;
	static const int Second = 1;
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = treeview_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
}
