using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Layout: Widget
	{
		private struct bounds_t
		{
			public int x;	
			public int y;	
			public int w;	
			public int h;	
			public IntPtr owner;

			public bounds_t(int x,int y,int w,int h)
			{
				this.x = x;
				this.y = y;
				this.w = w;
				this.h = h;
				this.owner = IntPtr.Zero;
			}
		}

		[DllImport(Global.ClaroGraphics, EntryPoint="layout_create")]
		private static extern IntPtr layout_create_raw(IntPtr parent, string layout_spec, bounds_t bounds, int min_w, int min_h);

		private static  IntPtr layout_create(IntPtr parent, string layout_spec, bounds_t bounds, int min_w, int min_h)
		{
			IntPtr handle = layout_create_raw(parent, layout_spec, bounds, min_w, min_h);
			if(handle == IntPtr.Zero)
				throw new ArgumentException("layout");
			return handle;	
		}
		
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr lt_bounds(IntPtr lt, string name);

		public Layout(IntPtr raw): base(raw)
		{
		}

		public Layout(Widget parent,string layout,Bounds bounds,int min_w,int min_h):
			base(layout_create(parent.Handle,layout,new bounds_t(bounds.X,bounds.Y,bounds.Width,bounds.Height),min_w,min_h))
		{
		}

		public Bounds GetBounds(string name)
		{
			return new Bounds(lt_bounds(Handle,name));	
		}
	}
}
