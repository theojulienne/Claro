CFLAGS= -I/sw/include/cairo -I/opt/local/include/cairo -I/usr/local/include/cairo -g `pkg-config gtk+-2.0 --cflags`
LDFLAGS= -L ../../src/claro/base/ -lclaro-base -L ../../src/claro/graphics/ -lclaro-graphics `pkg-config gtk+-2.0 --libs`

#-framework Cocoa  -dynamic -lclaro-graphics

all: test1

LIB_SRC=claro/core.d claro/base/*.d claro/graphics/*.d \
claro/graphics/widgets/*.d \
claro/graphics/cairo/*.d \
claro/graphics/cairooo/*.d

TEST1_SRC=test1.d $(LIB_SRC)
TEST1_OUT=test1
test1: $(TEST1_SRC)
	gdc $(TEST1_SRC) -o $(TEST1_OUT) $(LDFLAGS) $(CFLAGS)
