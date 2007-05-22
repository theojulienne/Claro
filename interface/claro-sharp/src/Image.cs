using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Image: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr image_load( IntPtr parent, string file );

		public Image(IntPtr raw): base(raw)
		{
		}
		
		public Image(Object parent, string file): this(image_load(parent.Handle,file))
		{
		}

		public Image(string file): this(image_load(IntPtr.Zero,file))
		{
		}
				
	}
}
