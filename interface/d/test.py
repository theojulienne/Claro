from _claro import *
import time

base_init( )
graphics_init( )

class ClockWindow(WindowWidget):
	def __init__( self ):
		WindowWidget.__init__( self, None, Bounds( 0, 0, 300, 300 ) )
		
		self.title = "Canvas Clock using PyD"
		
		self.lt = Layout( self, "[][_<|clock|<][]", 10, 10 )
		
		addToLoop( self.handle_main )
		
		self.add_handler( "destroy", self.closed )
		
		self.make_menu( )
		self.canvas = CanvasWidget( self, self.lt.bounds("clock") )
		self.canvas.redraw_delegate = self.draw_cb
		
		self.last_time = 0
	
	def draw_cb( self, w, c ):
		print c
	
	def make_menu( self ):
		self.mb = MenuBarWidget( self )
		file_menu = self.mb.appendItem( None, None, "File" );
		file_quit = self.mb.appendItem( file_menu, Stock.getImage("system-log-out", 0), "Quit" )
		self.mb.addKeyBinding( file_quit, "Q", 0 )
		file_quit.add_handler( "pushed", self.closed )
	
	def closed( self, evt, obj ):
		shutdown( )
	
	def handle_main( self, evt, obj ):
		t = int(time.time())
		
		if t > self.last_time:
			self.last_time = t
			self.canvas.redraw( )
		
		self.last_time = t

w = ClockWindow( )
w.show()
w.focus()

loop()