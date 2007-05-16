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
    def!(Claro.init, "base_init");
    def!(Claro.Graphics.init, "graphics_init");
    def!(Claro.loop);
    def!(Claro.shutdown);
    
    module_init();
    
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
}


