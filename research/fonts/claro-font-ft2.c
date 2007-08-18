#include <claro/base.h>
#include <claro/graphics.h>

#include <glib.h>

#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

// I should add a hashtable of cached fonts into the system design at some point for performance reasons.

typedef struct
{
    FcConfig * config;
    claro_list_t * font_families;
} claro_ft2_backend;

static claro_font_backend_t * claro_ft2_init()
{
    claro_ft2_backend * backend;
    FcBool res;

    backend = (claro_ft2_backend *)g_malloc(sizeof(claro_ft2_backend));

    res = FcInit();
    g_assert(res == TRUE);

    backend->config = FcConfigGetCurrent();
    g_assert(backend->config != NULL);
    
        
    return (claro_font_backend_t *)backend;
}

static void claro_ft2_cleanup(claro_font_backend_t * backend)
{
    claro_list_foreach(    
}


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


