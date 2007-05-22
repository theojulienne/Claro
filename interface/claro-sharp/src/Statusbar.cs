using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Statusbar: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr statusbar_widget_create( IntPtr parent, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern void statusbar_set_text( IntPtr obj, string text );

		public Statusbar(IntPtr raw): base(raw)
		{
		}

		public Statusbar(Widget parent): base(statusbar_widget_create(parent.Handle,0),parent)
		{
		}
	
		public string Text
		{
			set
			{
				statusbar_set_text(Handle,value);
			}
		}
	}
}
