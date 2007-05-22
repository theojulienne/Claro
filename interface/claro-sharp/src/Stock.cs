using System;
using System.Runtime.InteropServices;

namespace Claro
{
    public enum StockSize
    {
        Menu = 1,
        Tb = 2
    }

	public static class Stock
	{
        [DllImport(Global.ClaroGraphics)]
		private static extern IntPtr stock_get_image(string stock_id, StockSize size);

        public static Image GetImage(string stock_id, StockSize size)
        {
            IntPtr raw = stock_get_image(stock_id, size);
            if(raw == IntPtr.Zero)
                throw new ArgumentException("Invalid stock ID", stock_id);
            return new Image(raw);
        }    				
	}
}
