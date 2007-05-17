module claro.graphics.stock;

private import claro.base.objectsys;
private import claro.graphics.image;

extern (C)
{
	image_t *stock_get_image( char *stock_id, int flags );
}

enum StockSize
{
    Menu = 1,
    Tb = 2
}

class Stock {
	this() {
		throw new Exception("Cannot initiate class directly");
	}
	
	static Image getImage( char[] stock_id, StockSize size ) {
		return new Image( stock_get_image( std.string.toStringz(stock_id), size ), null );
	}
}
