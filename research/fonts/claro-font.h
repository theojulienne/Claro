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

#ifndef CLARO_GRAPHICS_FONT_H
#define CLARO_GRAPHICS_FONT_H

#include <claro/base.h>
#include <claro/graphics.h>

#include <cairo/cairo.h>

// This header file is really a mess and will be rewritten with the new object system.
// And the internal crap will be pulled out of course.

#define claro_type_HEAD 

#define cFontSlantNormal 0
#define cFontSlantItalic 1

#define cFontWeightNormal 0
#define cFontWeightBold 1

#define cFontDecorationNormal 0
#define cFontDecorationUnderline 1

// all objects to support properties 
    
typedef struct
{
    claro_type_HEAD
    
    void * native;
} claro_font_pattern_t;

// This will be a total mess on Gtk+ because of Pango's over-complicated design.
// Basically we will have to just assume Gtk+ is using FreeType.
typedef struct 
{
    claro_type_HEAD

    // The pattern that specfically matches this font.   
    claro_font_pattern_t * pattern;

    // an already matched FcPattern, a ATSUFontId, or a HFONT
    // for Pango, this will be the PangoFont* - then an FT_Face will be pulled out
    void * native;
} claro_font_t;

typedef struct
{
    claro_type_HEAD

    void * native;
} claro_fontset_t;

typedef void claro_fontset_foreach_func(claro_font_t * font, void * arg);

// not an object - just a structure for your crud..
typedef void claro_font_backend_t;


typedef struct
{
    //main functions

    // Initializes the font system and retrieves the list of available font families.
    // A font mapping cache should be created and stored in the backend object.
    // should g_assert on failure. (FUBAR)
    claro_font_backend_t * (* init) ();

    // Cleans up all allocated memory by the font system and unrefs any fonts in the font cache. 
    void (* cleanup) (claro_font_backend_t * backend);

    // Returns a global list of family names in UTF-8. DO NOT MODIFY.
    claro_list_t * (* get_font_families) (claro_font_backend_t * backend);

    // Attempts to load the font described by the arguments given. Possibly returning a similar font, or NULL. All can be missing except for family. It will also take the language into account.
    claro_font_t * (* load_font) (claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id);

    claro_fontset_t * (* load_fontset) (claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id);

     // Takes a Claro font and creates a Cairo font object for use with a Cairo context.
    cairo_font_face_t * (* create_cairo_font) (claro_font_backend_t * backend, claro_font_t * font);
   
    // Sets the widget's font.
    bool_t (* set_widget_font) (claro_font_backend_t * backend, widget_t * widget, claro_font_t * font);

    //fontset functions

    // references
    claro_fontset_t * (* ref_fontset) (claro_fontset_t * pattern);

    void (* unref_fontset) (claro_fontset_t * pattern);

    // enumeration
    int (* claro_fontset_count) (claro_fontset_t * fontset);

    claro_font_t * (* claro_fontset_get_item) (claro_fontset_t * fontset, int i);

    void (* claro_fontset_foreach) (claro_fontset_t * fontset, claro_fontset_foreach_func * func,
void * arg);

    //pattern functions

    // allocation
    claro_font_pattern_t * (* create_pattern) ();

    // references
    claro_font_pattern_t * (* ref_pattern) (claro_font_pattern_t * pattern);

    void (* unref_pattern) (claro_font_pattern_t * pattern);

    // gets - NULL or -1 means missing
    const char * (* get_family) (claro_font_pattern_t * pattern);

    int (* get_size) (claro_font_pattern_t * pattern);
    
    int (* get_weight) (claro_font_pattern_t * pattern);

    int (* get_slant) (claro_font_pattern_t * pattern);

    int (* get_decoration) (claro_font_pattern_t * pattern);

    //sets - set NULL or -1 to remove
    void (* set_family) (claro_font_pattern_t * pattern, const char * family);

    void (* set_size) (claro_font_pattern_t * pattern, int size);
    
    void (* set_weight) (claro_font_pattern_t * pattern, int weight);

    void (* set_slant) (claro_font_pattern_t * pattern, int slant);

    void (* set_decoration) (claro_font_pattern_t * pattern, int decoration);
    
} cgraphics_font_vtable;

#endif

