package.name = "claro-graphics"
package.kind = "dll"
package.language = "c"
package.includepaths = { "../../" }
package.links = { "claro-base", "claro-graphics-widgets", "claro-graphics-platform" }

package.files = {
	"layout/layout_parser.c",
	"layout/layout.c",
	"layout/layout_heap.c",
	"widget.c",
	"graphics.c",
	"image.c",
	"font.c",
	"system.c",
	"status_icon.c"
}

if (options["enable_cairo"]) then
	configureCairo(package)
else
	tinsert(package.defines, "NO_CAIRO")
end

