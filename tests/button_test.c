#include "tests.h" 

object_t 	*b1, *b2, *b3, *b4, *b5, *l1, *l2, *l3,*t1;


void close_window(object_t *window, event_t *event) {
	claro_shutdown(); 
}

void b4_pushed( object_t *object, event_t *event) {
	static int c = 0;
	static char *lines[] = { 
		"Thanks for pressing the button!  ('pushed' event)", 
		"Yup, this definately works..('pushed' event)", 
		"Are we having fun? ('pushed' event)"
	}; 
	
			
	label_set_text(l2, lines[c]); 
	
	c++; if (c>2) c=0;  
}

void b2_pushed(object_t *object, event_t *event) {
	button_set_text(b1, textbox_get_text(t1)); 
}

int main(int argc, char ** argv)
{

	object_t *w;
	layout_t *lt;
	bounds_t *b;

		
	claro_base_init();
	claro_graphics_init();
	
	w = window_widget_create(0, new_bounds(100,100,800, (35+5)*4.5) , 0);
	 
	object_addhandler(w, "destroy", close_window); 
	window_set_title(w, "Button Test"); 

	b = new_bounds(100,100,800,35*4+20);
	lt = layout_create(w, 
									"[{10}]"
									"[(10) {35}|(250)b1|(5)|t1|(5)|(120)b2|(10)]"
									"[{5}]"
									"[(10,35)|(75)b3|(5)|l1|(10)]"
									"[{5}]"
									"[(10,35)|(75)b4|(5)|l2|(10)]"
									"[{5}]"
									"[(10,35)|b5|(10)]"
									"[{10}]", *b, 0,0); 
									 
	b1 = button_widget_create(w, lt_bounds(lt, "b1"),0);
	t1 = textbox_widget_create(w,    lt_bounds(lt, "t1"),0); 
	b2 = button_widget_create_with_label(w, lt_bounds(lt, "b2"), 0, "Set button Text"); 
	b3 = button_widget_create_with_label(w, lt_bounds(lt, "b3"), 0, "B2"); 
	l1 = label_widget_create_with_text(w, lt_bounds(lt, "l1"), 0, "Button created via button_widget_create_with_label()"); 
	b4 = button_widget_create_with_label(w, lt_bounds(lt, "b4"), 0, "Press Me!"); 
	l2 = label_widget_create(w, lt_bounds(lt, "l2"),0); 
	b5 = button_widget_create_with_label(w, lt_bounds(lt, "b5"), 0, "Quit"); 

			
	object_addhandler(b2, "pushed", b2_pushed);
	object_addhandler(b4, "pushed", b4_pushed);
	object_addhandler(b5, "pushed", close_window); 
	 		
	window_show(w); 
	window_focus(w); 
			
	claro_loop(); 

    return 0;
}
