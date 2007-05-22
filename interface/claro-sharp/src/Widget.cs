using System;
using System.Text;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Widget: Claro.Object
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern void widget_show(IntPtr widget);

		[DllImport(Global.ClaroGraphics)]
		private static extern void widget_hide(IntPtr widget);

		[DllImport(Global.ClaroGraphics)]
		private static extern void widget_enable(IntPtr widget);

		[DllImport(Global.ClaroGraphics)]
		private static extern void widget_disable(IntPtr widget);

		[DllImport(Global.ClaroGraphics)]
		private static extern void widget_focus(IntPtr widget);

		internal Widget(IntPtr handle): base(handle)
		{
		}
	
		public Widget(IntPtr handle,Widget parent): base(handle,parent)
		{
		}

		public event Claro.EventHandler DestroyEvent;		

		internal override void AddHandlers()
		{		
			Connect("destroy", OnDestroyEvent);
		}

		protected virtual void OnDestroyEvent(IntPtr widget,IntPtr evt)
		{
			if(DestroyEvent != null)
					DestroyEvent(Widget.Wrap(widget),new Event(evt));
		}

		public virtual void Show()
		{
			widget_show(Handle);
		}			
		
		public virtual void Hide()
		{
			widget_hide(Handle);
		}

		public virtual void Focus()
		{
			widget_focus(Handle);
		}

		public virtual bool Enabled
		{
			set
			{
				if(value)
					widget_enable(Handle);
				else
					widget_disable(Handle);
			}
		}

		public Bounds SizeReq
		{
			get
			{
				return new Bounds(Marshal.ReadIntPtr(Handle,Claro.Object.Size));
			}
		}
	}
}
