module claro.base.objectsys;

import std.string;
import std.stdio;
import std.boxer;

struct object_t {}
struct event_t {}

alias void delegate(CEvent,CObject) event_handler_t;

struct DelegateStruct {
	event_handler_t dg;
}

DelegateStruct*[int] delegates;
int delegate_num = 1;

// allocates a new delegate, saves it, and returns the index
int createAndSaveDelegate(event_handler_t dg) {
	auto s = new DelegateStruct;
	int i = delegate_num++;
	
	delegates[i] = s;
	s.dg = dg;
	
	return i;
}

void dropDelegateReference( int index ) {
	delegates.remove( index );
}

extern (C) {
	extern object_t *claro;
	
	object_t *get_claro_object( ) {
		return claro;
	}
	
	void object_addhandler_interface( object_t *object, char *event, void function(object_t*,event_t*,void*) func, void *data );
	
	// function is always the pointer passed to addhandler, passes on to delegate
	void run_delegate( object_t *object, event_t *event, void *data ) {
		int dindex = cast(int)data;
		DelegateStruct *ds = delegates[dindex];
		event_handler_t func = ds.dg;
		
		auto evt = new CEvent( );
		evt.evt = event;
		
		auto obj = new CObject( );
		obj.obj = object;
		
		func( evt, obj );
	}
	
	void *event_get_ptr( event_t *e, char *arg );
	int event_get_int( event_t *e, char *arg );
	double event_get_double( event_t *e, char *arg );
}


class CEvent {
	event_t *evt;
	
	this()
	{
	    //
	}
	
	void *getArgumentAsPointer( char[] arg ) {
		return event_get_ptr( this.evt, std.string.toStringz(arg) );
	}
	
	Object getArgumentAsObject( char[] arg ) {
		object_t *obj = cast(object_t *)getArgumentAsPointer( arg );
		
		return CObject.forObject( obj );
	}
	
	int getArgumentAsInt( char[] arg ) {
		return event_get_int( this.evt, std.string.toStringz(arg) );
	}
	
	double getArgumentAsDouble( char[] arg ) {
		return event_get_double( this.evt, std.string.toStringz(arg) );
	}
}

// storage space to store object_t->CObject map
CObject c2d[object_t *];

class CObject {
	object_t *_obj;
	CObject parent;
	
	Box appdata;
	
	static CObject forObject( object_t *o )
	{
		if ( !( o in c2d ) )
			return null;
		
		return c2d[o];
	}
	
	void obj( object_t *_o )
	{
		_obj = _o;
		c2d[this._obj] = this;
	}
	
	object_t *obj( )
	{
		return _obj;
	}
	
	this( CObject parent ) {
		this.parent = parent;
	}
	
	this() {}
	
	void add_handler( char[] name, event_handler_t handler ) {
		if(this.obj is null)
		    return;
		int d = createAndSaveDelegate(handler);
		object_addhandler_interface( this.obj, std.string.toStringz(name), &run_delegate, cast(void*)d );
	}
	
	static object_t* getObj( CObject o ) {
		if ( o is null )
			return null;
		
		return o.obj;
	}
}
