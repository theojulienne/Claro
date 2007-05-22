using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public static class Application
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern void claro_loop();

		[DllImport(Global.ClaroGraphics)]
		private static extern void claro_shutdown();

		[DllImport(Global.ClaroGraphics)]
		private static extern void claro_base_init();
        
		[DllImport(Global.ClaroGraphics)]
		private static extern void claro_graphics_init();

		public static void Init()
		{
			claro_base_init();
			claro_graphics_init();
		}

		public static void Run()
		{
			claro_loop();
		}

		public static void Quit()
		{
			claro_shutdown();
		}
	}
}
