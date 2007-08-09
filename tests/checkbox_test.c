#include <claro/base.h>
#include <claro/graphics.h>


event_handler( close_window ) {
	claro_shutdown(); 
}

// checkbox A events 
void bToggleChecked_pushed(object_t *object, event_t *event, void *data) {
	object_t *c = (object_t*)data;
	
	checkbox_set_checked(c, 1-checkbox_get_checked(c)); 
}

void bChangeText_pushed(object_t *object, event_t *event,  void *data) {
	object_t *c = (object_t*)data; 
	
	static int x = 0; 
	static char *lines[] = {	"Changed text of checkbox via checkbox_set_label()", 
													"Yup, changing text of that checkbox via checkbox_set_label() still works!", 
													"Are we having fun changing the checkbox label via checkbox_set_label() ?"
												};
												
	checkbox_set_label(c, lines[x]);
	 
	x++; if (x>2) x=0; 
}

void cCheck_changed(object_t *object, event_t *event, void *data) {
	object_t *lStatus = (object_t*)data; 
	
	label_set_text(lStatus, (checkbox_get_checked(object) == 0 ? "Not checked" : "Checked!")); 
}



int main(int argc, char ** argv)
{

	object_t *w, *bQuit;
	
	object_t *cA, *lStatusA, *bChangeTextA, *bToggleCheckedA; 
	object_t *cB, *lStatusB, *bChangeTextB, *bToggleCheckedB;
	object_t *lA, *lB, *lC, *lD; 
	  
	layout_t *lt;
	bounds_t *b;
		
	claro_base_init();
	claro_graphics_init();

	// window
		// objects 
		w = window_widget_create(0, new_bounds(100,100,800, 230) , 0);
		b = new_bounds(100,100,800,230);
		 
     	lt = layout_create(w,	"[{20}]"
   												"[{25}<bleh|(5)|lA|(5)|(125)lStatusA|(5)]"
   												"[{35}<bleh|(5)|(125)bChangeTextA|(5)|(125)bToggleCheckedA|(5)|(525)cA|(5)]"
   												"[{30}]"
   												"[{25}<bleh|(5)|lB|(5)|(125)lStatusB|(5)]"
   												"[{35}<bleh|(5)|(125)bChangeTextB|(5)|(125)bToggleCheckedB|(5)|(525)cB|(5)]"
   												"[{20}]"
   												"[{35}bQuit]"
   												"[{5}]"
   												, *b, 0,0);

		bQuit = button_widget_create_with_label(w, lt_bounds(lt, "bQuit"), 0, "Quit"); 
		
		// events 
		object_addhandler(bQuit, "pushed", close_window); 
		object_addhandler(w, "destroy", close_window);
   							
 
		
	// checkbox A
		// objects 
		lA = label_widget_create_with_text(w,lt_bounds(lt, "lA"),0, "Checkbox A (checkbox_widget_create_with_text()) :");
		lStatusA = label_widget_create_with_text(w,lt_bounds(lt, "lStatusA"),0, "Not Checked");
	 	
		bChangeTextA = button_widget_create_with_label(w, lt_bounds(lt, "bChangeTextA"), 0, "Change Text");
		bToggleCheckedA = button_widget_create_with_label(w, lt_bounds(lt, "bToggleCheckedA"), 0, "Toggle Checked");
		cA = checkbox_widget_create_with_label(w, lt_bounds(lt, "cA"), 0, "Checkbox A");
		
		// events 
		object_addhandler_interface(bChangeTextA, "pushed", bChangeText_pushed, cA);	// change text 
		object_addhandler_interface(bToggleCheckedA, "pushed", bToggleChecked_pushed, cA);	// toggle
		object_addhandler_interface(cA, "changed", cCheck_changed, lStatusA);
	
	// checkbox B
		// objects 
		lB = label_widget_create_with_text(w,lt_bounds(lt, "lB"),0, "Checkbox B (checkbox_widget_create()) :");
		lStatusB = label_widget_create_with_text(w,lt_bounds(lt, "lStatusB"),0, "Not Checked");
	 	
		bChangeTextB = button_widget_create_with_label(w, lt_bounds(lt, "bChangeTextB"), 0, "Change Text");
		bToggleCheckedB = button_widget_create_with_label(w, lt_bounds(lt, "bToggleCheckedB"), 0, "Toggle Checked");
		cB = checkbox_widget_create(w, lt_bounds(lt, "cB"), 0); 
		
		// events 
		object_addhandler_interface(bChangeTextB, "pushed", bChangeText_pushed, cB);	// change text 
		object_addhandler_interface(bToggleCheckedB, "pushed", bToggleChecked_pushed, cB);	// toggle
		object_addhandler_interface(cB, "changed", cCheck_changed, lStatusB);
 

	
	// set things 
	window_set_title(w, "Checkbox Test");
	 		
	window_show(w); 
	window_focus(w); 
			
	claro_loop(); 

    return 0;
}
