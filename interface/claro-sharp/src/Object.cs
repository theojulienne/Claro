using System;
using System.Text;
using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;

/*
	char type[64];
	
	int destroy_pending;
	
	list_t event_handlers;
	
	list_t children;
	
	struct object_ *parent;
*/

namespace Claro
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	internal delegate void EventFunc( IntPtr obj, IntPtr evt );

	public class Object: IWrapper
	{	
		IntPtr m_handle;

		string m_type = null;
		Object m_parent = null;

		[DllImport(Global.ClaroGraphics)]
		private static extern void object_addhandler( IntPtr obj, string evt, IntPtr func);
	
		private static readonly Dictionary<string,Type> claro_types = GetTypes();

		private static Dictionary<string,Type> GetTypes()
		{
			Dictionary<string,Type> hash = new Dictionary<string,Type>();
			foreach(Type t in typeof(Object).Assembly.GetTypes())
				hash[t.FullName] = t;
			return hash;
		}

		private static Hashtable Objects = new Hashtable();

		internal static Object Wrap(IntPtr o)
		{
			if (o == IntPtr.Zero)
				return null;

			Object obj;
			WeakReference weak_ref = Objects[o] as WeakReference;
			if (weak_ref != null && weak_ref.IsAlive) {
				obj = weak_ref.Target as Claro.Object;
				return obj;
			}

			obj = CreateObject(o); 
			if (obj == null)
				return null;

			Objects [o] = new WeakReference (obj);
			return obj;
		}

		private static Claro.Object CreateObject(IntPtr raw)
		{
            //this is some serious voodoo- it will be replace in the new object system once it is finalized.
			Type type = null;
			try
			{
				StringBuilder sb = new StringBuilder(64);
				string typename = Marshal.PtrToStringAnsi(Marshal.ReadIntPtr(raw));
                string[] split = typename.Split('_');
				sb.Append(split[0]);			
				sb[0] = Char.ToUpper(sb[0]);
				sb.Insert(0,"Claro.");
				type = claro_types[sb.ToString()];
			}
			catch(System.IndexOutOfRangeException)
			{
				type = typeof(Claro.Object);	
			}
			catch(System.Collections.Generic.KeyNotFoundException)
			{
				type = typeof(Claro.Object);
			}
			return ( type.GetConstructor(new Type[]{typeof(IntPtr)}).Invoke(new object[]{raw}) as Object );
		}

		~Object()
		{
			Objects.Remove(Handle);
		}

		public Object(IntPtr handle)
		{
			if(handle == IntPtr.Zero)
				throw new ArgumentException("handle");
			
            m_handle = handle;
			AddHandlers();
		}

		public Object(IntPtr handle, Object parent): this(handle)
		{
			m_parent = parent;
		}

		public virtual Object Parent
		{	
			get
			{
				return m_parent;
			}
		}

		internal virtual void AddHandlers()
		{
		}

		internal void Connect(string evt,EventFunc func)
		{
			object_addhandler(m_handle, evt, Marshal.GetFunctionPointerForDelegate (func) );
		}

	/*	
        internal IntPtr make_offset(IntPtr base_ptr, int offset)
		{
			if(IntPtr.Size == 8)
				return new IntPtr( (long)offset + base_ptr.ToInt64() );
			else
				return new IntPtr( offset + base_ptr.ToInt32() );
		}

		public unsafe Claro.Object[] Children
		{
			get
			{
				List raw = new List(make_offset(m_handle,72+(IntPtr.Size*2)));
				int n = raw.Length;
				IntPtr[] nodes = raw.Iter();
				Claro.Object[] children = new Claro.Object[n];
				for(int i = 0; i < n; i++)
					children[i] = Claro.Object.Wrap(nodes[i]);
				return children;
			}
		} 
	*/
		public string TypeName
		{
			get 
			{
				if(m_type == null) 	
				{				
					m_type = Marshal.PtrToStringAnsi(m_handle);	
				}
				return m_type;			
			}
		}

		public IntPtr Handle
		{
			get 
			{
				return m_handle;
			}
		}
	}
}
