using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Window: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr window_widget_create( IntPtr parent, IntPtr bounds, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern void window_set_title(IntPtr window, string title);
	
		[DllImport(Global.ClaroGraphics)]
		private static extern void window_set_icon(IntPtr window, IntPtr icon);

		[DllImport(Global.ClaroGraphics)]
		private static extern void window_show(IntPtr window);

		[DllImport(Global.ClaroGraphics)]
		private static extern void window_hide(IntPtr window);
	
		[DllImport(Global.ClaroGraphics)]
		private static extern void window_focus(IntPtr window);
	
		public Window(IntPtr raw): base(raw)
		{
		}

		public Window(Widget parent,Bounds bounds,int flags): base(window_widget_create(parent.Handle,bounds.Handle,flags))
		{
		}

		public Window(Bounds bounds, int flags): base(window_widget_create(IntPtr.Zero,bounds.Handle,flags))
		{
		}
	
		public Window(Bounds bounds, bool resize): base(window_widget_create(IntPtr.Zero,bounds.Handle,resize?0:4))
		{
		}

		public Window(Bounds bounds): this(bounds,0)
		{
		}

		public override void Show()
		{
			window_show(Handle);
		}

		public override void Hide()
		{
			window_hide(Handle);
		}

		public override void Focus()
		{
			window_focus(Handle);
		}

		public string Title
		{
			set
			{
				window_set_title(Handle,value);
			}
		}

		public Image Icon
		{
			set
			{
				window_set_icon(Handle,value.Handle);
			}
		}
		
	}
}
