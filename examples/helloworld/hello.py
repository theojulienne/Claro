#!/usr/bin/env python

import claro

w = t = b = 0

def push_my_button( obj, event ):
	t.set_text( "You pushed my button!" )
	obj.set_text( "Ouch!" )

claro.base.init( )
claro.graphics.init( )

claro.base.log( claro.base.CL_INFO, "hello.py running using Claro!" )

w = claro.graphics.window( None, 100, 100, 230, 230, 0 )
w.set_title( "Hello, World!" )

t = claro.graphics.textbox( w, 10, 10, 210, -1, 0 )
t.set_text( "Yeehaw!" )

b = claro.graphics.button( w, 40, 45, 150, -1, 0 )
b.set_text( "Push my button!" )

b.addhandler( "pushed", push_my_button )

w.show( )
w.focus( )

claro.base.loop( )
