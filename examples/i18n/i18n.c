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

#define UTF8_TEXT   " 日本語: 猿も木から落ちる。\n\n 中文：\n 山中问答\n\n"\ 
                    " 问余何意栖碧山\n 笑而不大心自闲\n"\
                    " 桃花流水窅然去\n 别有天地在人间\n\n 李白\n\n" 

object_t *w, *t;

static void window_closed(object_t *window, event_t *event) 
{
    claro_shutdown();
}

int main( int argc, char *argv[] )
{
        claro_base_init( );
        claro_graphics_init( );

        log_fd_set_level( CL_DEBUG, stderr );

        clog( CL_INFO, "%s running using Claro!", __FILE__ );

        w = window_widget_create( 0, new_bounds( 100, 100, 230, 230 ), 0 );
        window_set_title( w, "claro i18n test" );
        object_addhandler(w, "destroy", window_closed);

        t = label_widget_create_with_text(w, new_bounds(0, 0, 230, 230), 0, UTF8_TEXT);

        window_show( w );
        window_focus( w );

        block_heap_loginfo( );

        claro_loop( );

        return 0;
}


