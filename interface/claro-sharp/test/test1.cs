using System;
using Claro;

public class Test
{
	public static void Main()
	{
		Application.Init();
		Claro.EventHandler on_quit = delegate { Application.Quit(); };		

		Bounds b = new Bounds(100,100,250,250);
		Window w = new Window(b);
		w.Title = "test";
		w.DestroyEvent += on_quit;

		/*
        Button but = new Button(w, new Bounds(0, 10, 100, 35));
		but.Text = "click me!";		
		but.Pushed += delegate(object sender, Claro.Event evt) 
		{ 
			(sender as Claro.Button).Text = "i was clicked!"; 
		};
        */

        Canvas c = new Canvas(w, new Bounds(0, 20, 150, 150));
        		

		Menubar mb = new Menubar(w);
		MenuItem file = mb.AppendItem("File");
		MenuItem file_quit = file.AppendItem("_Quit", Stock.GetImage("system-log-out", StockSize.Menu));
        file_quit.Pushed += on_quit;
        mb.AddKeyBinding(file_quit, "Q", MenuKeyModifier.Command);

		w.Show();
		w.Focus();
		Application.Run();
	}
}
