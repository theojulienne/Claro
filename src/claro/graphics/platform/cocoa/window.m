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
////#include <cairo-quartz.h>
#include "../macosx_cocoa.h"

/* ClaroWindow (subclassed from NSWindow) */

@implementation ClaroWindow

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self contentRectForFrameRect:[self frame]];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
	widget_set_content_size( cw, frame.size.width, frame.size.height, 1 );
}

- (void)claroMove:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
	widget_set_content_position( cw, 0, 0, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (void)setClaroWidget:(widget_t *)widget
{
	SaneView *sv;
	cw = OBJECT(widget);
	
	/* thanks, but no thanks: a normal coordinate system would be nice. */
	sv = [[SaneView alloc] initWithFrame: [[self contentView] frame]];
	
	if ( !( widget->flags & cWidgetCustomDraw ) )
	{
		[sv setBackgroundColor: [NSColor windowBackgroundColor]];
	}
	else
	{
		[sv setBackgroundColor: [NSColor clearColor]];
	}
	
	[self setContentView:sv];
	[sv release];
	
	[self setAcceptsMouseMovedEvents: YES];
	
	/* event handlers */
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSWindowDidResizeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSWindowDidMoveNotification
		object:self];
}

@end

@implementation ClaroPanelWindow

- (void)claroResize:(NSNotification *)aNotification
{
	NSRect frame = [self contentRectForFrameRect:[self frame]];
	widget_set_size( cw, frame.size.width, frame.size.height, 1 );
	widget_set_content_size( cw, frame.size.width, frame.size.height, 1 );
}

- (void)claroMove:(NSNotification *)aNotification
{
	NSRect frame = [self frame];
	widget_set_position( cw, frame.origin.x, frame.origin.y, 1 );
	widget_set_content_position( cw, 0, 0, 1 );
}

- (void)claroClose:(NSNotification *)aNotification
{
	widget_destroy( cw );
}

- (BOOL) canBecomeKeyWindow
{
    return YES;
}

- (void)setClaroWidget:(widget_t *)widget
{
	SaneView *sv;
	cw = OBJECT(widget);
	
	/* thanks, but no thanks: a normal coordinate system would be nice. */
	sv = [[SaneView alloc] initWithFrame: [[self contentView] frame]];

	if ( !( widget->flags & cWidgetCustomDraw ) )
	{
		[sv setBackgroundColor: [NSColor windowBackgroundColor]];
	}
	else
	{
		[sv setBackgroundColor: [NSColor clearColor]];
	}

	[self setContentView:sv];
	[sv release];
	
	[self setAcceptsMouseMovedEvents: YES];
	
	/* event handlers */
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroClose:) name:NSWindowWillCloseNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroResize:) name:NSWindowDidResizeNotification
		object:self];
	
	[[NSNotificationCenter defaultCenter] addObserver:self
		selector:@selector(claroMove:) name:NSWindowDidMoveNotification
		object:self];
}

@end

/* called when a window widget needs creating */
void cgraphics_window_widget_create( widget_t *widget )
{
	int m = 0;
	
	m |= NSTitledWindowMask;
	m |= NSClosableWindowMask;
	
	if ( !( widget->flags & cWindowFixedSize ) )
		m |= NSResizableWindowMask;
	
	if ( widget->flags & cWindowToolStyle )
		m |= NSUtilityWindowMask;
	
	if ( widget->flags & cWidgetCustomDraw )
	{
		m = NSBorderlessWindowMask;
	}
	
	//m |= NSTexturedBackgroundWindowMask;
	//m |= NSUtilityWindowMask;
	
	if ( widget->flags & cWindowToolStyle )
	{
		ClaroPanelWindow *window = [[ClaroPanelWindow alloc]
			initWithContentRect: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
			styleMask: m			       
			backing: NSBackingStoreBuffered
			defer: NO
			];
	
		[window setClaroWidget:widget];
	
		widget_set_content_size( OBJECT(widget), widget->size_req->w, widget->size_req->h, 1 );
		
		[window setLevel: NSFloatingWindowLevel];
	
		widget->native = (NSControl *)(void *)window;
	}
	else
	{
		ClaroWindow *window = [[ClaroWindow alloc]
			initWithContentRect: NSMakeRect( widget->size_req->x, widget->size_req->y, widget->size_req->w, widget->size_req->h )
			styleMask: m			       
			backing: NSBackingStoreBuffered
			defer: NO
			];
	
		if ( widget->flags & cWidgetCustomDraw )
		{
			/* This code is a TEST ONLY. This will probably change later */
			[window setBackgroundColor: [NSColor clearColor]];
			[window setLevel: NSStatusWindowLevel];
			[window setAlphaValue:1.0];
			[window setOpaque:NO];
			[window setHasShadow: YES];
		}
	
		[window setClaroWidget:widget];
	
		widget_set_content_size( OBJECT(widget), widget->size_req->w, widget->size_req->h, 1 );
	
		if ( widget->flags & cWindowFloating )
			[window setLevel: NSFloatingWindowLevel];
		else
			[window makeKeyWindow];
	
		widget->native = (NSControl *)(void *)window;
	}
}

void cgraphics_window_show( window_widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->widget.native;
	
	if ( WIDGET(w)->flags & cWindowModalDialog && OBJECT(w)->parent != NULL &&
		object_is_of_class( OBJECT(w)->parent, "window_widget" ) )
	{
		[NSApp beginSheet: window
			modalForWindow: WIDGET(OBJECT(w)->parent)->native
			modalDelegate: nil
			didEndSelector: nil
			contextInfo: nil];
	}
	else
	{
		[window display];
		[window orderFrontRegardless];
	}
}

void cgraphics_window_close( widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->native;
	
	if ( WIDGET(w)->flags & cWindowModalDialog && OBJECT(w)->parent != NULL &&
		object_is_of_class( OBJECT(w)->parent, "window_widget" ) )
	{
		[NSApp endSheet: window];
		[window orderOut: window];
		
		[window setReleasedWhenClosed: YES];
		[window close];
	}
	else
	{
		[window setReleasedWhenClosed: YES];
		[window close];
		//[window setEnabled:NO];
	}
}

void cgraphics_window_hide( window_widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->widget.native;
	[window orderOut:nil];
}

void cgraphics_window_focus( window_widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->widget.native;
	
	[window makeKeyAndOrderFront: window];
}

void cgraphics_window_update_title( window_widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->widget.native;
	NSString *str;

	str = [[NSString alloc] initWithCString:w->title encoding:NSUTF8StringEncoding];
	
	[window setTitle:str];
	
	[str release];
}

void cgraphics_window_minimise( widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->native;
	
	[window orderOut:nil];
}

void cgraphics_window_maximise( widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->native;
	
	[window display];
	[window orderFrontRegardless];
	// now maximise?
}

void cgraphics_window_restore( widget_t *w )
{
	ClaroWindow *window = (ClaroWindow *)w->native;
	
	[window display];
	[window orderFrontRegardless];
}

void cgraphics_window_update_icon( widget_t *w )
{
	
}

