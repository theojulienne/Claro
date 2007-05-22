using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Event: IWrapper
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr event_get_arg_ptr( IntPtr evt, int arg );

		[DllImport(Global.ClaroGraphics)]
		private static extern int event_get_arg_int( IntPtr evt, int arg );

		IntPtr m_handle;
		string m_name = null;

		public Event(IntPtr handle)
		{
			if(handle == IntPtr.Zero)
				throw new ArgumentException("handle");
			m_handle = handle;
		}

		public Claro.Object Target
		{
			get
			{
				IntPtr obj = Marshal.ReadIntPtr(m_handle,0);
				return new Claro.Object(obj);
			}
		}

		public string EventName
		{
			get 
			{
				if(m_name == null) 	
				{				
					IntPtr raw = make_offset(m_handle,IntPtr.Size);
					m_name = Marshal.PtrToStringAnsi(raw);	
				}
				return m_name;			
			}
		}

		public IntPtr Handle
		{
			get 
			{
				return m_handle;
			}
		}

		internal IntPtr make_offset(IntPtr base_ptr, int offset)
		{
			if(IntPtr.Size == 8)
				return new IntPtr( (long)offset + base_ptr.ToInt64() );
			else
				return new IntPtr( offset + base_ptr.ToInt32() );
		}

		public object[] Args
		{
			get
			{
				int arg_num = Marshal.ReadInt32(m_handle,IntPtr.Size+68);
				char[] format = Marshal.PtrToStringAnsi(make_offset(m_handle,IntPtr.Size+72)).ToCharArray();
				object[] args = new object[arg_num];
				for(int i = 0; i < arg_num; i++)
				{
					if(format[i] == 'i')
						args[i] = event_get_arg_int(Handle,i);
					else if(format[i] == 'p')
						args[i] = event_get_arg_ptr(Handle,i);
				}
				return args;
			}
		}
	}
}
