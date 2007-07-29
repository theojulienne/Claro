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
//#include <cairo-quartz.h>
#include "../macosx_cocoa.h"

void cgraphics_image_load( image_t *img, const char *file )
{
	NSString *str;

	str = [[NSString alloc] initWithCString:file encoding:NSUTF8StringEncoding];
	
	img->native = [[NSImage alloc] initByReferencingFile:str];
	
	[str release];
}

void cgraphics_image_load_inline_png( image_t *img, const unsigned char * data, int len )
{
	img->native = [[NSImage alloc] initWithData:[NSData dataWithBytes:data length:len]];
}
