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


#include "../win32.h"

/* Textarea */

void cgraphics_textarea_widget_create( widget_t *widget )
{
	object_t *object = (object_t *)widget;
	widget_t *parent = (widget_t *)object->parent;
	HWND hwnd, hwnd_parent = widget_get_container(parent);
	
	/* FIXME: bad default */
	if ( widget->size_req->h == -1 )
		widget->size_req->h = 25;

	// ( flags & C_BORDER_NONE ? 0 : WS_EX_CLIENTEDGE )
		
	if ( !( hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, "EDIT", 0,
	                               WS_CHILD | ES_AUTOHSCROLL | WS_TABSTOP | ES_MULTILINE |
	                               0, //(flags & C_TEXTBOX_PASSWORD ? ES_PASSWORD : 0),
	                               widget->size_req->x, widget->size_req->y,
	                               widget->size_req->w, widget->size_req->h,
	                               hwnd_parent,
	                               NULL, (HINSTANCE) GetModuleHandle( NULL ), NULL ) ) )
		MessageBox( 0, "Could not create text HWND.", "Claro error", 0 );
	
	widget->native = hwnd;
	widget->ndata = NULL;

	ShowWindow( hwnd, SW_SHOW );
	UpdateWindow( hwnd );
}


void cgraphics_textarea_set_text( widget_t *widget, const char *text )
{
	HWND hwnd = widget->native;
	SetWindowText( hwnd, text );
}

char *cgraphics_textarea_get_text( widget_t *widget )
{
	int text_length = GetWindowTextLength( widget->native ) + 1;
	LPTSTR str = g_new0( TCHAR, text_length );
	GetWindowText( widget->native, str, text_length );
	// FIXME: This is actually incorrect. If compiled with UNICODE, THIS WILL FAIL.
	// TODO: convert string to utf8 when UNICODE is set
	return str;
}
