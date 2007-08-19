all:
	gcc test.c claro-font.c claro-font-ft2.c -I ../../src `pkg-config gtk+-2.0 --cflags --libs` -o test ../../src/claro/base/libclaro-base.a ../../src/claro/graphics/libclaro-graphics.a

clean:
	rm *~ -rf
	rm test
