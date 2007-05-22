using System;
using System.Runtime.InteropServices;

namespace Claro
{
	public class Canvas: Widget
	{
		[DllImport(Global.ClaroGraphics)]
		private static extern IntPtr canvas_widget_create( IntPtr parent, IntPtr bounds, int flags );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_redraw( IntPtr obj );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_show_text( IntPtr widget, int x, int y, string text, int len );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_set_text_color( IntPtr widget, double r, double g, double b, double a );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_set_text_bgcolor( IntPtr widget, double r, double g, double b, double a );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_fill_rect( IntPtr widget, int x, int y, int w, int h, double r, double g, double b, double a );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_draw_image( IntPtr widget, IntPtr image, int x, int y );

		[DllImport(Global.ClaroGraphics)]
		private static extern void canvas_set_text_font( IntPtr widget, string face, int size, int weight, int slant, int decoration );

		public Canvas(IntPtr raw): base(raw)
		{
		}

		public Canvas(Widget parent,Bounds bounds): base(canvas_widget_create(parent.Handle,bounds.Handle,0),parent)
		{
		}

		private static Claro.EventFunc on_redraw;

		private static void redraw_cb(IntPtr widget,IntPtr evt)
		{
			Canvas c = (Object.Wrap(widget) as Canvas);
			Console.WriteLine("on_redraw() c = {0}, raw = {1}", c.GetHashCode(), widget);
			//OnRedraw(c,null);
			c.FillRect(0,0,100,100,1,1,1,1);
		}

		internal override void AddHandlers()
		{
			if(on_redraw == null)
				on_redraw = new Claro.EventFunc(redraw_cb);

			Connect("redraw", on_redraw);
		}

		protected void OnRedraw(object widget,Claro.Event evt)
		{
			if(Redraw != null)
				Redraw(widget,evt);
		}

		public void ForceRedraw()
		{
			canvas_redraw(Handle);
		}

		public void ShowText(int x,int y,string text)
		{
			canvas_show_text(Handle,x,y,text,text.Length);
		}

		public void SetTextColor(double r,double g,double b,double a)
		{
			canvas_set_text_color(Handle,r,g,b,a);
		}
	
		public void SetTextBgColor(double r,double g,double b,double a)
		{
			canvas_set_text_bgcolor(Handle,r,g,b,a);
		}
		
		public void FillRect(int x,int y,int w,int h,double r,double g,double b,double a)
		{
			canvas_fill_rect(Handle,x,y,w,h,r,g,b,a);
		}

		public void DrawImage(int x,int y,Claro.Image img)
		{
			canvas_draw_image(Handle,img.Handle,x,y);
		}

		public void SetTextFont(string face,int size,bool bold,bool italic,bool underline)
		{
			canvas_set_text_font(Handle,face,size,bold?1:0,italic?1:0,underline?1:0);
		}

		public event Claro.EventHandler Redraw;	

	}
}
