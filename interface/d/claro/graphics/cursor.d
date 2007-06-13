module claro.graphics.cursor;

extern (C)
{
	void claro_cursor_capture( );
	void claro_cursor_release( );
	void claro_cursor_hide( );
	void claro_cursor_show( );
}

class Cursor
{
	static void capture( )
	{
		claro_cursor_capture( );
	}
	
	static void release( )
	{
		claro_cursor_release( );
	}
	
	static void show( )
	{
		claro_cursor_show( );
	}
	
	static void hide( )
	{
		claro_cursor_hide( );
	}
}
