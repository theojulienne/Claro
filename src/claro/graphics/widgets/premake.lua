package.name = "claro-graphics-widgets"
package.kind = "lib"
package.language = "c"
package.includepaths = { "../../../" }

package.files = 
{
	"stock.c",
	"stock-16.c",
	"stock-22.c",
	"window.c",
	"toolbar.c",
	"textbox.c",
	"button.c",
	"canvas.c",
	"splitter.c",
	"container.c",
	"checkbox.c",
	"dialog.c",
	"label.c",
	"progress.c",
	"radio.c",
	"statusbar.c",
	"textarea.c",
	"frame.c",
	"image.c",
	"list.c",
	"listbox.c",
	"listview.c",
	"combo.c",
	"menubar.c",
	"menu.c",
	"scrollbar.c",
	"workspace.c",
	"opengl.c",
	"treeview.c",
	"font_dialog.c"
}	

if (options["enable_cairo"]) then
	configureCairo(package)
else
	tinsert(package.defines, "NO_CAIRO")
end

