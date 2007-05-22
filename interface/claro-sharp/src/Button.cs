using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Button: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr button_widget_create( IntPtr parent, IntPtr bounds, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern void button_set_label( IntPtr obj, string text );

		public Button(IntPtr raw): base(raw)
		{
		}

		public Button(Widget parent,Bounds bounds): base(button_widget_create(parent.Handle,bounds.Handle,0),parent)
		{
		}
	
		public Button(Widget parent,Bounds bounds,string label): this(parent,bounds)
		{
			Text = label;
		}

		internal override void AddHandlers()
		{
			Connect("pushed", OnPushed);
		}

		public event Claro.EventHandler Pushed;	

		protected virtual void OnPushed(IntPtr widget,IntPtr evt)
		{
			if(Pushed != null)
					Pushed(Widget.Wrap(widget),new Event(evt));
		}

		public string Text
		{
			set
			{
				button_set_label(Handle,value);
			}
		}
	}
}
