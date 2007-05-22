using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Progress: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr progress_widget_create( IntPtr parent, IntPtr bounds, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern void progress_set_level( IntPtr obj, double percent );

		public Progress(IntPtr raw): base(raw)
		{
		}
	
		public Progress(Widget parent,Bounds bounds): base(progress_widget_create(parent.Handle,bounds.Handle,0),parent)
		{
		}

		public double Level
		{
			set
			{
				progress_set_level(Handle,value);
			}
		}
	}
}
