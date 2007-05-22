using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Claro
{
	public class List: IWrapper
	{
		internal static readonly int Size = (IntPtr.Size * 2) + 4;

		private unsafe struct node
		{
			public node * next;
			public node * prev;
			public void * data;
		}	

		private unsafe struct list
		{
			public node * head;
			public node * tail;
			public int count;
		}
	
		IntPtr m_handle;

		public List(IntPtr handle)
		{
			if(handle == IntPtr.Zero)
				throw new ArgumentException("handle");
			m_handle = handle;
		}

		public IntPtr Handle
		{
			get 
			{
				return m_handle;
			}
		}
	
		public unsafe int Length
		{
			get
			{
				return ((list*)m_handle.ToPointer())->count;	
			}
		}
	
		public unsafe IntPtr[] Iter()
		{
			List<IntPtr> nodes = new List<IntPtr>();
			list * children = (list*)m_handle.ToPointer();		
			for(node * iter = children->head;iter != null;iter = iter->next)
				nodes.Add((IntPtr)iter->data);
			return nodes.ToArray();
		}
	}
}
