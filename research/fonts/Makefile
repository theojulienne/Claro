all:
	gcc test.c claro-font.c claro-font-ft2.c -I ../../src `pkg-config gtk+-2.0 --cflags --libs` -o test -L ../../src/claro/base -lclaro-base -L ../../src/claro/graphics -lclaro-graphics

clean:
	rm *~ -rf
	rm test
