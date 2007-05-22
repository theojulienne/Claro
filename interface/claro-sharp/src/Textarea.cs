using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Textarea: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr textarea_widget_create( IntPtr parent, IntPtr bounds, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern void textarea_set_text( IntPtr obj, string text );

		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr textarea_get_text( IntPtr obj );

		public Textarea(IntPtr raw): base(raw)
		{
		}

		public Textarea(Widget parent,Bounds bounds): base(textarea_widget_create(parent.Handle,bounds.Handle,0),parent)
		{
		}

		internal override void AddHandlers()
		{
			Connect("changed", OnChanged);
		}

		public event Claro.EventHandler Changed;	

		protected virtual void OnChanged(IntPtr widget,IntPtr evt)
		{
			if(Changed != null)
					Changed(Widget.Wrap(widget),new Event(evt));
		}

		public string Text
		{
			set
			{
				textarea_set_text(Handle,value);		
			}

			get
			{
				IntPtr raw = textarea_get_text(Handle);
				return Marshal.PtrToStringAnsi(raw);
			}
		}
	}
}
