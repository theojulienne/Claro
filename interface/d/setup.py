import os
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
    
setup(
    name=projName,
    version='0.1',
    ext_modules=
        [
        Extension(projName, getSourcefiles(), libraries=["claro-base", "claro-graphics"],
            library_dirs=["../../src/claro/base/", "../../src/claro/graphics/"])
        ],
  )

