module claro.graphics.widgets.slider;

import std.stdio;
import std.string;

private import claro.graphics.widget;

extern (C) {
	object_t *slider_widget_create( object_t *parent, bounds_t *bounds, int flags );
	void slider_set_range( object_t *slider, double minimum, double maximum );
	void slider_set_value( object_t *slider, double value );
	void slider_set_style( object_t *slider, int style );
	double slider_get_value( object_t *slider );
}

enum cSliderStyle {
	cSliderHorizontal = 0x0,
	cSliderVertical = 0x01,
	cSliderKnob = 0x02,
}

class SliderWidget : Widget {
	double _minimum = 0;
	double _maximum = 1;
	
	this( CObject parent, Bounds b, int flags ) {
		super( parent );
		
		this.b = b;
		this.obj = slider_widget_create( CObject.getObj(parent), this.b.obj, flags );
		this.created( );
	}
	
	this( CObject parent, Bounds b ) {
		this( parent, b, 0 );
	}
	
	void minimum( double d ) {
		_minimum = d;
		slider_set_range( this.obj, _minimum, _maximum );
	}
	
	void maximum( double d ) {
		_maximum = d;
		slider_set_range( this.obj, _minimum, _maximum );
	}
	
	double value( )
	{
		return slider_get_value( this.obj );
	}
	
	void value( double d )
	{
		slider_set_value( this.obj, d );
	}
	
	void style( int s )
	{
		slider_set_style( this.obj, s );
	}
}
