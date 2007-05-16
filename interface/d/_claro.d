module _claro;

import claro.core;
import claro.base.all;
import claro.graphics.all;

import std.stdio;
import std.math;
import std.c.time;
import std.string;

import python;
import pyd.pyd;

extern(C) void PydMain() 
{   
	// FIXME: these should be nicely class'd up too
	def!(Claro.init, "base_init");
	def!(Claro.Graphics.init, "graphics_init");
	def!(Claro.addToLoop, "addToLoop");
	def!(Claro.loop);
	def!(Claro.shutdown);
	
	module_init();
	/*
	wrap_class!(
		Claro,
		StaticDef!(Claro.init),
		StaticDef!(Claro.addToLoop),
		StaticDef!(Claro.loop),
		StaticDef!(Claro.shutdown)
	);
*/
	wrap_class!(
		CEvent,
		Def!(CEvent.getArgumentAsPointer)
	);
	
	wrap_class!(
		CObject,
		Def!(CObject.add_handler)
	);
	
	wrap_class!(
		Bounds,
		Init!(void function(int, int, int, int)),
		Property!(Bounds.x),
		Property!(Bounds.y),
		Property!(Bounds.w),
		Property!(Bounds.h)
	);
	
	wrap_class!(
		Layout,
		Init!(	void function(CObject, char[], Bounds, int, int),
		 		void function(CObject, char[], int, int)),
		Def!(Layout.bounds)
	);
	
	wrap_class!(
		Widget,
		Def!(Widget.created),
		Def!(Widget.bounds)
	);
	
	wrap_class!(
		WindowWidget,
		Init!(	void function(CObject, Bounds),
				void function(CObject, Bounds, int)),
		Def!(WindowWidget.show),
		Def!(WindowWidget.focus),
		Property!(WindowWidget.title)
	);
	
	wrap_class!(
		ButtonWidget,
		Init!(	void function(CObject, Bounds),
				void function(CObject, Bounds, int),
				void function(CObject, Bounds, int, char[])),
		Property!(ButtonWidget.label)
	);
	
	wrap_class!(
		CanvasWidget,
		Init!(	void function(CObject, Bounds),
				void function(CObject, Bounds, int)),
		Def!(CanvasWidget.redraw),
		Property!(CanvasWidget.redraw_delegate)
	);
    
/*
    wrapped_class!(CEvent, "event") event;
    event.def!(CEvent.getArgumentAsPointer);
    finalize_class(event);
    
    wrapped_class!(CObject, "object") object;
    object.def!(CObject.add_handler);
    finalize_class(object);
    
    wrapped_class!(Bounds, "bounds") bounds;
    bounds.init!(void function(int, int, int, int));
    bounds.def!(Bounds.x);
    bounds.def!(Bounds.y);
    bounds.def!(Bounds.w);
    bounds.def!(Bounds.h);
    finalize_class(bounds);
    
    wrapped_class!(Widget, "widget") widget;
    widget.def!(Widget.created);
    widget.def!(Widget.bounds);
    finalize_class(widget);

	wrapped_class!(WindowWidget, "WindowWidget") window_widget;
	window_widget.init!(void function(CObject, Bounds));
    window_widget.def!(WindowWidget.show);
    window_widget.def!(WindowWidget.focus);
    finalize_class(window_widget);*/
}


