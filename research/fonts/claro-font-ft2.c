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

#include <claro/base.h>
#include <claro/graphics.h>

#include <glib.h>

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

// I know my ADD error checking here might seem a bit excessive..but ghost bugs waste man hours.


// I should add a hashtable of cached fonts into the system design at some point for performance reasons.
typedef struct
{
    FcConfig * config;
    claro_list_t * font_families;
} claro_ft2_backend;

// helpers

static void _claro_ft2_font_pattern_destroy(void * obj)
{
    claro_font_pattern_t * pattern = (claro_font_pattern_t *)obj;

    FcPatternDestroy((FcPattern *)pattern->native);
}

static void _claro_ft2_font_destroy(void * obj)
{
    claro_font_t * font = (claro_font_t *)obj;

    claro_type_unref(font->pattern);
}

// This also validates each font pattern so we don't have to twice.
static int _cmp_font_family(const void * a, const void * b)
{
    FcChar8 * family_a, * family_b;
    FcResult res;

    g_assert(a != NULL && b != NULL);

    res = FcPatternGetString (* (FcPattern **)a, FC_FAMILY, 0, &family_a);
    g_assert(res == FcResultMatch); 

    res = FcPatternGetString (* (FcPattern **)b, FC_FAMILY, 0, &family_b);
    g_assert(res == FcResultMatch);    
    
    return g_strcasecmp(family_a, family_b);
}

static FcBool _claro_ft2_list_families(claro_ft2_backend_t * backend)
{
    FcPattern * pattern;
    FcFontSet * set;
    FcObjectSet * os;
    int i;

    pattern = FcPatternCreate();
    g_return_val_if_fail(pattern != NULL, FALSE);

    os = FcObjectSetCreate();
    g_return_val_if_fail(FcObjectSetAdd(os, FC_FAMILY), FALSE);

    set = FcFontList(backend->config, pattern, os);
    g_return_val_if_fail(set != NULL, FALSE);

    qsort((void *)set->fonts, set->nfont, sizeof(void *), _cmp_font_family);

    for(i = 0; i < set->nfont; i++)
    {
        FcChar8 * family;
        FcPattern * font = set->fonts[i];
        FcPatternGetString (font, FC_FAMILY, 0, &family);

        claro_list_append(backend->font_families, sstrdup(family));
    }

    FcFontSetDestroy(set);
    FcObjectSetDestroy(os);
    FcPatternDestroy(pattern);

    return FALSE;
}

static claro_font_pattern_t * _claro_ft2_make_pattern(FcPattern * fc_pattern)
{
    claro_font_pattern_t * pattern;

    g_assert(fc_pattern != NULL);

    pattern = (claro_font_pattern_t *)smalloc(sizeof(claro_font_pattern_t));

    claro_type_init(pattern, _claro_ft2_font_pattern_destroy);

    if(fc_pattern == NULL)
        pattern->native = (void *)FcPatternCreate();    
    else
    {
        FcPatternReference(fc_pattern);
        pattern->native = (void *)fc_pattern;
    }

    return pattern;
}

static claro_font_t * _claro_ft2_make_font(FcPattern * fc_pattern)
{
    claro_font_pattern_t * pattern;
    claro_font_t * font;    

    g_assert(fc_pattern != NULL);
    
    font = (claro_font_t *)smalloc(sizeof(claro_font_t));
    claro_type_init(font, _claro_ft2_font_destroy);

    font->pattern = _claro_ft2_make_pattern(real_pattern);

    // This is a bit goofy..because we don't need to deal with FreeType directly.
    font->native = NULL;

    claro_type_ref(font->pattern);

    return font;
}

// vtable

static claro_font_backend_t * claro_ft2_init()
{
    claro_ft2_backend * ft2_backend;
    FcBool res;

    ft2_backend = (claro_ft2_backend *)smalloc(sizeof(claro_ft2_backend));

    res = FcInit();
    g_assert(res == TRUE);

    ft2_backend->config = FcConfigGetCurrent();
    g_assert(backend->config != NULL);
    
    ft2_backend->font_families = claro_list_create();
    res = _claro_ft2_list_families(ft2_backend);
    g_assert(res == TRUE);        

    return (claro_font_backend_t *)ft2_backend;
}

static void claro_ft2_cleanup(claro_font_backend_t * backend)
{
    int i, len;
    claro_ft2_backend * ft2_backend = (claro_ft2_backend *)backend;
    
    g_return_val_if_fail(ft2_backend != NULL, NULL);

    len = claro_list_count(ft2_backend->font_families);

    for(i = 0; i < len; i++)
        sfree(claro_list_get_item(ft2_backend->font_families, i));

    claro_list_destroy(ft2_backend->font_families);

    sfree(ft2_backend);   
}

static claro_list_t * claro_ft2_get_font_families(claro_font_backend_t * backend)
{
    claro_ft2_backend * ft2_backend = (claro_ft2_backend *)backend;
    
    g_return_val_if_fail(ft2_backend != NULL, NULL);
    
    return ft2_backend->font_families;
}
   
claro_font_t * claro_ft2_load_font(claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id)
{
    claro_ft2_backend * ft2_backend = (claro_ft2_backend *)backend;
    FcPattern * real_pattern = NULL, * test_pattern;
    FcResult res;
    
    claro_font_t * font;    
    
    g_return_val_if_fail(ft2_backend != NULL, NULL);
    g_return_val_if_fail(pattern != NULL, NULL);
    // lang_id can be unspecified

    test_pattern = (FcPattern *)pattern->native;

    if(lang_id)
        FcPatternAddString (test_pattern, FC_LANG, lang_id);

    g_return_val_if_fail(FcConfigSubstitute(config, test_pattern, FcMatchPattern), NULL);
    FcDefaultSubstitute(test_pattern);

    real_pattern = FcFontMatch(config, test_pattern, &res);                
    if(res != FcResultMatch)
    {
        FcPatternDestroy(real_pattern);     
        return NULL; 
    }
   
    font = _claro_ft2_make_font(real_pattern);

    return font;
}

static claro_fontset_t * claro_ft2_load_fontset(claro_font_backend_t * backend, claro_font_pattern_t * pattern, const char * lang_id)
{
    //TODO
    return NULL;
}

// Takes a Claro font and creates a Cairo font object for use with a Cairo context.
static cairo_font_face_t * claro_ft2_create_cairo_font(claro_font_backend_t * backend, claro_font_t * font)
{
    g_return_val_if_fail(backend != NULL, NULL);
    g_return_val_if_fail(font != NULL, NULL);
    return cairo_ft_font_face_create_for_pattern((FcPattern *)font->pattern);   
}
   
// Sets the widget's font.
static bool_t set_widget_font(claro_font_backend_t * backend, widget_t * widget, claro_font_t * font)
{
    return TRUE;
    // Not dependent on fontconfig..deal with later.
}

static claro_fontset_t * claro_ft2_ref_fontset(claro_fontset_t * fontset)
{
    g_return_val_if_fail(fontset != NULL, NULL);

    claro_type_ref(fonset);
    return fonset;
}

static void unref_fontset(claro_fontset_t * pattern)
{
    g_return_if_fail(fontset != NULL);

    claro_type_unref(fontset);
}
   
static int claro_ft2_fontset_count(claro_fontset_t * fontset)
{
    FcFontSet * fc_fontset;
    
    g_return_val_if_fail(fontset != NULL, -1);

    fc_fontset = (FcFontSet *)fontset->native;

    return fc_fontset->nfont;
}

static claro_font_t * claro_ft2_fontset_get_item(claro_fontset_t * fontset, int i)
{
    FcFontSet * fc_fontset;
    FcPattern * fc_pattern;

    g_return_val_if_fail(fontset != NULL, NULL);

    fc_fontset = FcFontSet *)fontset->native;

    g_return_val_if_fail(i < 0 || i > fc_fontset->nfont, NULL);

    fc_pattern = fc_fontset->font[i];  
    
    g_return_val_if_fail(fc_pattern != NULL, NULL);

    return _claro_ft2_make_font(fc_pattern);
}

static void claro_fontset_foreach(claro_fontset_t * fontset, claro_fontset_foreach_func * foreach_func, void * arg)
{
    FcFontSet * fc_fontset;
    int i;

    g_return_val_if_fail(fontset != NULL, NULL);
    g_return_val_if_fail(func != NULL, NULL);
    
    fc_fontset = FcFontSet *)fontset->native;

    for(i = 0; i < fc_fontset->nfont; i++)
    {
        FcPattern * fc_pattern = fc_fontset->font[i]; 
        foreach_func(_claro_ft2_make_font(fc_pattern), arg);
    }
}

static claro_font_pattern_t * claro_ft2_create_pattern()
{
    return _claro_ft2_make_pattern(NULL);
}

static claro_font_pattern_t * (* ref_pattern) (claro_font_pattern_t * pattern);

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


