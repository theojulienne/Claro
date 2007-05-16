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

@interface CocoaStatusIcon: NSObject
{
	status_icon_t * status;
	NSStatusBar   * ns_bar;
	NSStatusItem  * ns_item;
	NSImage       * current_image;
	NSString      * ns_tooltip;
}
- (id) initWithStatusIcon:(status_icon_t *)status_icon;
- (void) ensureItem;
- (void) actionCb:(NSObject *)button;
- (void) setImage:(image_t *)image;
- (void) setVisible:(int)visible;
- (void) setToolTip:(const char *)tooltip_text;
@end

@implementation CocoaStatusIcon : NSObject
- (id) initWithStatusIcon:(status_icon_t *)status_icon
{
  [super init];
  status = status_icon;
  ns_bar = [NSStatusBar systemStatusBar];

  return self;
}

- (void) ensureItem
{
  if (ns_item != nil)
    return;

  ns_item = [ns_bar statusItemWithLength:NSVariableStatusItemLength];
  [ns_item setAction:@selector(actionCb:)];
  [ns_item setTarget:self];
  [ns_item retain];
}

- (void) dealloc
{
  [current_image release];
  [ns_item release];
  [ns_bar release];

  [super dealloc];
}

- (void) actionCb:(NSObject *)button
{ 
  NSEvent *event = [NSApp currentEvent];
  double time = [event timestamp];
  
  event_send( OBJECT(status), "pushed", "" );
}

- (void) setImage:(image_t *)image
{
  /* Support NULL */
  [self ensureItem];

  if (current_image != nil) 
  {
    [current_image release];
    current_image = nil;
  }
  
  if (!image) 
  {
    [ns_item release];
    ns_item = nil;
    return;
  }

  if(image->native)
  {
	current_image = image->native;
	[current_image retain];

	[ns_item setImage:current_image];
  }	
}

- (void) setVisible:(int)visible
{
  if (visible) 
  {
    [self ensureItem];
    if (ns_item != nil)
      [ns_item setImage:current_image];
    if (ns_tooltip != nil)
      [ns_item setToolTip:ns_tooltip];
  } else 
  {
    [ns_item release];
    ns_item = nil;
  }
}

- (void) setToolTip:(const char *)tooltip_text
{
  [ns_tooltip release];
  ns_tooltip = [[NSString stringWithUTF8String:tooltip_text] retain];
  
  [ns_item setToolTip:ns_tooltip];
}
@end

void cgraphics_status_icon_create(status_icon_t * status, int flags)
{
	//NEED AUTO-POOL RUBBISH HERE (AND EVERYWHERE ELSE IN COCOA :-P)
	status->native = (void*) [[CocoaStatusIcon alloc] initWithStatusIcon:status];
}

void cgraphics_status_icon_set_icon(status_icon_t * status, image_t * icon)
{
	CocoaStatusIcon * si = (CocoaStatusIcon*) status->native;
	[si setImage:icon];
}

void cgraphics_status_icon_set_menu(status_icon_t * status, object_t * menu)
{
	CocoaStatusIcon * si = (CocoaStatusIcon*) status->native;
	//FIXME
}

void cgraphics_status_icon_set_visible(status_icon_t * status, int visible)
{
	CocoaStatusIcon * si = (CocoaStatusIcon*) status->native;
	[si SetVisible:visible];
}

void cgraphics_status_icon_set_tooltip(status_icon_t * status, const char * tooltip)
{
	CocoaStatusIcon * si = (CocoaStatusIcon*) status->native;
	[si setToolTip:tooltip];
}
