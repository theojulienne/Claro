/* Claro Graphics - an abstraction layer for native UI libraries
 * 
 * $Id$
 * 
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 * 
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 * 
 * See the LICENSE file for more details.
 */

#define _PLATFORM_INC
#include <claro/graphics.h>
#include "../macosx_cocoa.h"

/* ClaroSlider (subclassed from NSSlider) */
@interface ClaroSlider : NSSlider
{
    object_t *cw;
}

/* internal init function */
- (void)setClaroWidget:(widget_t *)widget;

/* notification responders */
- (void)claroResize:(NSNotification *)aNotification;
- (void)claroClose:(NSNotification *)aNotification;

- (void)changed:(NSSlider*)sender;
@end

@implementation ClaroSlider

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (void)setClaroWidget:(widget_t *)widget
{
	cw = OBJECT(widget);
	
	[self setMinValue:0.0];
	[self setMaxValue:1.0];
	[self setDoubleValue:0.0];
	[self setTarget: self];
	[self setAction:@selector(changed:)];
	[self setContinuous: YES];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSViewFrameDidChangeNotification
		object:self];
}

- (void)changed:(NSSlider*)sender
{
	event_send( OBJECT(cw), "changed", "" );
}

@end



void cgraphics_slider_widget_create( widget_t *widget )
{
	ClaroSlider *s;
	NSView *parent = cgraphics_get_native_parent( widget );
	
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 20; // FIXME
	
	s = [[ClaroSlider alloc]
		initWithFrame: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
		];
	
	[s setClaroWidget:widget];
	[parent addSubview: s];
	
	widget->native = (NSControl *)s;
}

void cgraphics_slider_set_value( widget_t *slider, double value )
{
	ClaroSlider *s = (ClaroSlider *)slider->native;
	
	[s setDoubleValue: value];
}

void cgraphics_slider_set_range( widget_t *slider, double minimum, double maximum )
{
	ClaroSlider *s = (ClaroSlider *)slider->native;
	
	[s setMinValue: minimum];
	[s setMaxValue: maximum];
}

void cgraphics_slider_set_style( widget_t *slider, int style )
{
	ClaroSlider *s = (ClaroSlider *)slider->native;
	
}

double cgraphics_slider_get_value( widget_t *slider )
{
	ClaroSlider *s = (ClaroSlider *)slider->native;
	
	return [s doubleValue];
}
