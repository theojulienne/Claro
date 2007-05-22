using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Label: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr label_widget_create_with_text( IntPtr parent, IntPtr bounds, int flags , string text);

		[DllImport(Global.ClaroGraphics)]
		private static extern void label_set_text( IntPtr obj, string text );

		public Label(IntPtr raw): base(raw)
		{
		}

		public Label(Widget parent,Bounds bounds,string text): base(label_widget_create_with_text(parent.Handle,bounds.Handle,0,text),parent)
		{
		}

		public string Text
		{
			set
			{
				label_set_text(Handle,value);		
			}
		}
	}
}
