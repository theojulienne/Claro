package.name = "claro-graphics"
package.kind = "dll"
package.language = "c"
package.includepaths = { "../../" }

package.files = {
	"layout/layout_parser.c",
	"layout/layout.c",
	"layout/layout_heap.c",
	"widget.c",
	"graphics.c",
	"image.c",
	"font.c",
	"system.c",
	"status_icon.c",
	"widgets/stock.c",
	"widgets/stock-16.c",
	"widgets/stock-22.c",
	"widgets/window.c",
	"widgets/toolbar.c",
	"widgets/textbox.c",
	"widgets/button.c",
	"widgets/canvas.c",
	"widgets/splitter.c",
	"widgets/container.c",
	"widgets/checkbox.c",
	"widgets/dialog.c",
	"widgets/label.c",
	"widgets/progress.c",
	"widgets/radio.c",
	"widgets/statusbar.c",
	"widgets/textarea.c",
	"widgets/frame.c",
	"widgets/image.c",
	"widgets/list.c",
	"widgets/listbox.c",
	"widgets/listview.c",
	"widgets/combo.c",
	"widgets/menubar.c",
	"widgets/menu.c",
	"widgets/scrollbar.c",
	"widgets/workspace.c",
	"widgets/opengl.c",
	"widgets/treeview.c",
	"widgets/font_dialog.c",
}



function configureCairo(package)
	tinsert(package.links, { "cairo" })
	if (options["with-cairo-includes"]) then
		tinsert(package.includepaths, { options["with-cairo-includes"] })
	end
	if (OS == "macosx") then
		tinsert(package.includepaths, { "/usr/local/include" })
	end
end

addoption("enable-cairo", "Enable Cairo Graphics support")
addoption("with-cairo-includes", "Path to cairo graphics include")

if (options["enable_cairo"]) then
	configureCairo(package)
else
	tinsert(package.defines, "NO_CAIRO")
end