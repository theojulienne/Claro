#include <claro/base.h>
#include <claro/graphics.h>

#include "claro-font.h"

static void window_closed( object_t * obj, event_t * event )
{
	claro_shutdown();
}

static void enum_fonts(object_t * l)
{
    claro_list_t * list;
    int i, len;

    list = claro_get_font_families();
    len = claro_list_count(list);
    
    for(i = 0; i < len; i++)
        listbox_append_row(l, (char *)claro_list_get_item(list, i));
}

int main(int argc, char ** argv)
{
    object_t * w, *l;
    bounds_t *b;

    claro_base_init();
    claro_graphics_init();
    claro_fonts_init();

    b = new_bounds( 0, 0, 250, 400 );
    w = window_widget_create( 0, b, 0 );
	object_addhandler( w, "destroy", window_closed );
	window_set_title( w, "Font test" );    
    
    l = listbox_widget_create( w, b, 0 );
    
    enum_fonts(l);

    window_show( w );

    claro_loop();

    claro_fonts_cleanup();
    return 0;
}
