package.name = "claro-graphics-platform"
package.kind = "lib"
package.language = "c"
package.includepaths = { "../../../" }
    
function addpkgconfiglib (package, libname)
    if options.target == gnu and os.execute ("pkg-config --exists "..libname) == 0 then
      table.insert (package.buildoptions, "`pkg-config --cflags "..libname)
      table.insert (package.linkoptions, "`pkg-config --libs "..libname)
    else
      table.insert (package.linkoptions, findlib (libname))
    end
end
  
if (OS == "macosx") then
    print("OSX- FIXME")
elseif (OS == "windows") then
    print("WIN32- FIXME")
else
    addpkgconfiglib(package, "gtk+-2.0")
    package.files = 
    {
        "gtk.c"
    }
    print("found Gtk+..")
end

if (options["enable_cairo"]) then
	configureCairo(package)
else
	tinsert(package.defines, "NO_CAIRO")
end

