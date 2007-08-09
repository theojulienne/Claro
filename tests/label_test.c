#include <claro/base.h>
#include <claro/graphics.h>

object_t *lblText, *txtEntry;


event_handler( window_closed ) {
	claro_shutdown(); 
}

void handle_justify(object_t *object, event_t *event, void *data) {
	int f = (int)data; 
	
	label_set_justify(lblText, f); 
}

event_handler( handle_set_text ) {
	label_set_text(lblText, textbox_get_text(txtEntry)); 
}

int main(int argc, char ** argv)
{

	object_t *w, *f; 
	layout_t *lt, *lt2;
	bounds_t *b, *b2;
	
	object_t *btnSetText, *rbLeftJustify, *rbCenterJustify, *rbRightJustify, *rbFillJustify;
	object_t *rg, *btnQuit;  
	
	int y = 10, x= 10; 
	
   claro_base_init();
	claro_graphics_init();

	// create objects 	
	w = window_widget_create(0, new_bounds(100,100,500, 200) , 0); 
   lt = layout_create(w, 	"[{10}]"
   									"[{35}text|(5)|set]"
   									"[{5}]"
   									"[{35}lj|cj|rj|fj]"
   									"[{5}]"
   									"[{50}fr]"
   									"[{5}]"
   									"[{35}quit]"
   									"[{10}]"
   									, *b, 10,10);

	txtEntry = textbox_widget_create(w, lt_bounds(lt, "text"), 0);     
   btnSetText = button_widget_create_with_label(w, lt_bounds(lt, "set"), 0,"Set Text");
 
 	b2 = lt_bounds(lt, "fr"); 
 	f = frame_widget_create_with_label(w, b2, 0, "Label Test Area");
 	lt2 = layout_create(f, "[{5}][_label][{5}]", *b2, 10,10);
 	lblText = label_widget_create(f, lt_bounds(lt2, "label"), 0); 
 	 	
   rg = radiogroup_create(w,0);   
   rbLeftJustify = radiobutton_widget_create(w, rg, lt_bounds(lt, "lj"), "Left Justify",0); 
   rbCenterJustify = radiobutton_widget_create(w, rg, lt_bounds(lt, "cj"), "Center Justify",0);
   rbRightJustify = radiobutton_widget_create(w, rg, lt_bounds(lt, "rj"), "Right Justify",0);
	rbFillJustify = radiobutton_widget_create(w, rg, lt_bounds(lt, "fj"), "Fill Justify",0);
     
	btnQuit = button_widget_create_with_label(w, lt_bounds(lt, "quit"), 0, "Quit"); 
	
	// set up handlers 
	object_addhandler(w, "destroy", window_closed);
	object_addhandler(btnQuit, "pushed", window_closed);
	
	object_addhandler(btnSetText, "pushed", handle_set_text);   
	object_addhandler_interface(rbLeftJustify, "selected", handle_justify, (void*)cLabelLeft); 
	object_addhandler_interface(rbCenterJustify, "selected", handle_justify, (void*)cLabelCenter);
	object_addhandler_interface(rbRightJustify, "selected", handle_justify, (void*)cLabelRight);
	object_addhandler_interface(rbFillJustify, "selected", handle_justify, (void*)cLabelFill);
	
	
	// set things 
	window_set_title(w, "Label Test"); 
	
	 		
	window_show(w); 
	window_focus(w); 
			
	claro_loop(); 

    return 0;
}
