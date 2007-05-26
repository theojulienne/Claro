module claro.base.objectsys;

import std.string;
import std.stdio;

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
	
	int getArgumentAsInt( char[] arg ) {
		return event_get_int( this.evt, std.string.toStringz(arg) );
	}
}

class CObject {
	object_t *obj;
	CObject parent;
	
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
