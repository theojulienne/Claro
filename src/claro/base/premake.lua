package.name = "claro-base"
package.kind = "dll"
package.language = "c"
package.includepaths = { "../../" }

if (OS == "macosx") then
	tinsert( package.linkoptions, { "-dynamiclib" } )
end

if (OS ~= "windows") then
	tinsert( package.defines, { "HAVE_MMAP" } )
end

package.files = {
	"block.c",
	"hashtable.c",
	"claro.c",
	"list.c",
	"log.c",
	"memory.c",
	"object.c",
	"oscompat.c",
	"store.c",
	"object_map.c",
	"string.c",
}