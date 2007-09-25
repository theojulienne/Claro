import _claro, cairo, math

def on_quit(o, e):
	_claro.shutdown()

def on_click(o, e):
	x, y = e["x"], e["y"]
	print x,y
	
def on_redraw(o, e):
	cr = o.cr
	cr.scale(300, 300)
	
	cr.select_font_face ("Sans", cairo.FONT_SLANT_NORMAL, cairo.FONT_WEIGHT_BOLD)
	cr.set_font_size (0.3)

	cr.move_to (0.035, 0.53)
	cr.show_text ("Hello")

	cr.move_to (0.22, 0.65)
	cr.text_path ("void")
	cr.set_source_rgb (0.5,0.5,1)
	cr.fill_preserve ()
	cr.set_source_rgb (0,0,0)
	cr.set_line_width (0.01)
	cr.stroke ()

	#/* draw helping lines */
	cr.set_source_rgba (1,0.2,0.2, 0.6)
	cr.arc (0.04, 0.53, 0.02, 0, 2* math.pi)
	cr.arc (0.27, 0.65, 0.02, 0, 2* math.pi)
	cr.fill ()

def make_menu(m):
    file_menu = m.append_item(None, None, "&File")
    file_quit = m.append_item(file_menu, _claro.stock_get_image("system-log-out", _claro.cStockMenu), "&Quit")
    file_quit.addhandler("pushed", on_quit)
    m.add_key_binding(file_quit, "Q", _claro.cModifierCommand)

def on_selected(o, e):
    header.text = l_d[o.selected]

def make_listbox(l):
    l_d[l.append_row("One")] = "One"
    l_d[l.append_row("Two")] = "Two"

l_d = {}
w = _claro.Window(None, _claro.Bounds(0, 0, 300, 300), 0, "Claro Test")
m = _claro.MenuBar(w)
make_menu(m)
#c = _claro.Canvas(w, _claro.Bounds(0,0,300,300), 0)
header = _claro.Label(w, _claro.Bounds(0, 0, 300, 30))
header.text = "Bob"
l = _claro.ListBox(w, _claro.Bounds(0,30,300,270))
make_listbox(l)

w.addhandler("destroy", on_quit)
l.addhandler("selected", on_selected)
#c.addhandler("redraw", on_redraw)
#c.addhandler("clicked", on_click)

w.show()

_claro.loop()
