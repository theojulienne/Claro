all:
	gcc _claro.c  -shared -fPIC -o _claro.so -I /usr/include/python2.5 `pkg-config --cflags --libs gtk+-2.0` -I ../../src/ ../../src/claro/base/libclaro-base.a ../../src/claro/graphics/libclaro-graphics.a ../../src/claro/base/string.o -g 

