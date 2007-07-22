module claro.graphics.widget;

public import claro.base.objectsys;

struct bounds_t {
	int x, y;
	int w, h;
	
	object_t *owner;
}

struct widget_t {}

extern (C)
{
	bounds_t *new_bounds( int x, int y, int w, int h );
	bounds_t *get_req_bounds( object_t *widget );
	
	void widget_set_notify( object_t *widget, int flags );
	
	void widget_disable( object_t *widget );
	void widget_enable( object_t *widget );
	void widget_focus( object_t *widget );
}

class Bounds {
	bounds_t *obj;
	
	this()
	{
	    throw new Exception("Cannot initiate class directly");
	}
	
	this( int x, int y, int w, int h ) {
		this.obj = new_bounds( x, y, w, h );
	}
	
	this( bounds_t *obj ) {
		this.obj = obj;
	}
	
	int x( ) { return this.obj.x; }
	int y( ) { return this.obj.y; }
	int w( ) { return this.obj.w; }
	int h( ) { return this.obj.h; }

	int left( ) { return this.obj.x; }
	int top( ) { return this.obj.y; }
	int width( ) { return this.obj.w; }
	int height( ) { return this.obj.h; }
	
	// PyD expects properties to have setters
	int x( int _x ) { return this.obj.x; }
	int y( int _x ) { return this.obj.y; }
	int w( int _x ) { return this.obj.w; }
	int h( int _x ) { return this.obj.h; }
}

class Widget : CObject {
	Bounds b;
	
	static int NotifyMouse = 1;
	static int NotifyKey = 2;
	
	static int ModifierKeyAlternate=1; // alt or alt/option
	static int ModifierKeyControl=2; // control
	static int ModifierKeyCommand=4; // command/windows
	static int ModifierKeyShift=8; // shift
	
	this( CObject parent ) {
		super( parent );
	}
	
	this() { super(); }
	
	void created( ) {
		
	}
	
	Bounds bounds( ) {
		return new Bounds( get_req_bounds(this.obj) );
	}
	
	void notify( int flags )
	{
		widget_set_notify( this.obj, flags );
	}
	
	void disabled( bool val )
	{
		if ( val ) widget_disable( this.obj );
		else widget_enable( this.obj );
	}
	
	void enabled( bool val )
	{
		disabled = !val;
	}
	
	void focus( )
	{
		widget_focus( this.obj );
	}
}
