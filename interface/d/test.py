from _claro import *

base_init( )
graphics_init( )

class ClockWindow(WindowWidget):
	def __init__( self ):
		WindowWidget.__init__( self, None, Bounds( 0, 0, 300, 300 ) )
		
		self.title = "Canvas Clock using PyD"
		
		self.lt = Layout( self, "[][_<|clock|<][]", 10, 10 )
		
		#addToLoop( self.handle_main )
		
		self.add_handler( "destroy", self.closed )
		
		self.make_menu( )
		self.canvas = CanvasWidget( self, self.lt.bounds("clock") )
		#self.canvas.redraw_delegate = self.draw_cb
	
	def draw_cb( self, evt, obj ):
		pass
	
	def make_menu( self ):
		pass
	
	def closed( self, evt, obj ):
		shutdown( )
	
	def handle_main( self, evt, obj ):
		pass

w = ClockWindow( )
w.show()
w.focus()

loop()