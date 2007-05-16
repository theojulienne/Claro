import os, sys
from celerid.support import setup, Extension

projName = '_claro'
dirs = [
            "claro/",
            "claro/base/",
            "claro/graphics/",
            "claro/graphics/widgets/",
            "claro/graphics/cairo/",
            "claro/graphics/cairooo/"
          ]
          
def getSourcefiles():
    sources = []
    for dir in dirs:
        for f in os.listdir(dir):
            if f[-1:-3:-1] == "d.":
                sources.append(dir+f)   
    sources.append(projName + '.d')       
    return sources

def getPythonRuntimeLib():
    return 'python'+str(sys.version_info[0])+'.'+str(sys.version_info[1])

def getPythonLibraryDir():
    if sys.platform == 'darwin':
        return [ '/Library/Frameworks/Python.framework/Versions/Current/lib/'+getPythonRuntimeLib()+'/config' ]
    return []

setup(
    name=projName,
    version='0.1',
    ext_modules=
        [
        Extension(projName, getSourcefiles(), libraries=["claro-base", "claro-graphics", "cairo", getPythonRuntimeLib()],
            library_dirs=["../../src/claro/base/", "../../src/claro/graphics/"]+getPythonLibraryDir())
        ],
  )

