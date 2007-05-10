project.name = "Claro"
project.bindir = "bin"
project.libdir = "lib"

dopackage("src/claro/base")
dopackage("src/claro/graphics")
dopackage("src/claro/graphics/widgets")
dopackage("src/claro/graphics/platform")

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

