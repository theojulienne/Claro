using System;
using System.Runtime.InteropServices;

namespace Claro
{
	[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
	public delegate void EventHandler(object sender,Claro.Event evt);
}
