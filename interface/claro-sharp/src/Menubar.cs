using System;
using System.Runtime.InteropServices;

namespace Claro
{
    public enum MenuKeyModifier
    {
        Shift = 1 << 0,
        Command = 1 << 1
    }
    
	public class Menubar: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr menubar_widget_create( IntPtr parent, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr menubar_append_item( IntPtr menubar, IntPtr parent, IntPtr image, string title );
		
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr menubar_append_separator( IntPtr menubar, IntPtr parent );

        [DllImport(Global.ClaroGraphics)]
		private static extern void menubar_add_key_binding( IntPtr menubar, IntPtr item, string key,  MenuKeyModifier modifier);

		public Menubar(IntPtr raw): base(raw)
		{
		}

		public Menubar(Widget parent): base(menubar_widget_create(parent.Handle,0))
		{
		}	

        public void AddKeyBinding(MenuItem item, string key, MenuKeyModifier modifier)
        {
            menubar_add_key_binding(Handle, item.Handle, key, modifier);
        }

		public MenuItem AppendItem(string title)
		{
			IntPtr raw = menubar_append_item(Handle,IntPtr.Zero,IntPtr.Zero,title);
			return new MenuItem(raw,Handle);
		}

		public MenuItem AppendItem(string title,Image image)
		{
			IntPtr raw = menubar_append_item(Handle,IntPtr.Zero,image.Handle,title);
			return new MenuItem(raw,Handle);
		}

		public MenuItem AppendSeparator()
		{
			IntPtr raw = menubar_append_separator(Handle,IntPtr.Zero);
			return new MenuItem(raw,Handle);
		}
	}

	public class MenuItem: Claro.Object
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr menubar_append_item( IntPtr menubar, IntPtr parent, IntPtr image, string title );
		
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr menubar_append_separator( IntPtr menubar, IntPtr parent );

		private IntPtr m_bar;

		public MenuItem(IntPtr raw): base(raw)
		{
		}

		internal MenuItem(IntPtr raw,IntPtr bar): this(raw)
		{
			m_bar = bar;
		}

		internal override void AddHandlers()
		{
			Connect("pushed", OnPushed);
		}

		public event Claro.EventHandler Pushed;	

		protected virtual void OnPushed(IntPtr widget,IntPtr evt)
		{
			if(Pushed != null)
					Pushed(Widget.Wrap(m_bar),new Event(evt));
		}

		public MenuItem AppendItem(string title)
		{
			IntPtr raw = menubar_append_item(m_bar,Handle,IntPtr.Zero,title);
			return new MenuItem(raw,m_bar);
		}

		public MenuItem AppendItem(string title,Image image)
		{
			IntPtr raw = menubar_append_item(m_bar,Handle,image.Handle,title);
			return new MenuItem(raw,m_bar);
		}	
		
		public MenuItem AppendSeparator()
		{
			IntPtr raw = menubar_append_separator(m_bar,Handle);
			return new MenuItem(raw,m_bar);
		}
	}
}
