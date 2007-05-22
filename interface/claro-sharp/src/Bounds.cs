using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Bounds: IWrapper
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr new_bounds( int x, int y, int w, int h );

		[DllImport(Global.ClaroGraphics)]
		private static extern void bounds_set( IntPtr bounds, int x, int y, int w, int h );

		IntPtr m_handle;

		public Bounds(IntPtr handle)
		{
			if(handle == IntPtr.Zero)
				throw new ArgumentException("handle");
			m_handle = handle;
		}

		public Bounds(int x,int y,int w,int h): this(new_bounds(x,y,w,h))
		{
		}

		public void Set(int x,int y,int w,int h)
		{
			bounds_set(m_handle,x,y,w,h);
		}

		public IntPtr Handle
		{
			get 
			{
				return m_handle;
			}
		}
	
/*
	int x, y;
	int w, h;
	object_t *owner;
*/
		public int X
		{
			get
			{
				return Marshal.ReadInt32(Handle,(4*0));
			}
		}
		
		public int Y
		{
			get
			{
				return Marshal.ReadInt32(Handle,(4*1));
			}
		}

		public int Width
		{
			get
			{
				return Marshal.ReadInt32(Handle,(4*2));
			}
		}

		public int Height
		{
			get
			{
				return Marshal.ReadInt32(Handle,(4*3));
			}
		}
	}
}
