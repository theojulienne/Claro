using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Claro
{
	internal static class Global
	{
        [DllImport(Global.ClaroBase)]
		private static extern IntPtr smalloc(int size);

        [DllImport(Global.ClaroBase)]
		private static extern void sfree(IntPtr p);

        [DllImport("msvcrt")]
		private static extern int strlen(IntPtr p);

		public const string ClaroGraphics = "claro-graphics";
        public const string ClaroBase = "claro-base";
        internal const string GLib = "glib-2.0";
        internal static readonly bool IsRealGLib = CheckForLinux();

        private static bool CheckForLinux()
        {
            return true;    
        }
    
        public unsafe static IntPtr StringToUtf8Ptr(string s)
        {
            if(s == null)
                throw new ArgumentException("s");           
            
            int len = s.Length;
            IntPtr raw = smalloc(len);
            int utf8_len = Encoding.UTF8.GetByteCount(s);
            fixed(char * cptr = s)
            {
                Encoding.UTF8.GetBytes(cptr, len, (byte*)raw.ToPointer(), utf8_len);
            }            
            return raw;
        }

        public static unsafe string Utf8PtrToString(IntPtr p, bool needs_free)
        {
            if(p == IntPtr.Zero)
                throw new ArgumentException("p");
            //byte * bptr = (byte*)p.ToPointer();
            int utf8_len = strlen(p);
                        
            if(needs_free)
                sfree(p);
        }	
    }
}


