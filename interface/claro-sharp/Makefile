CSC=gmcs
all:
	$(CSC) src/*.cs -unsafe -out:Claro.dll -t:library

test1: all
	$(CSC) test/test1.cs -out:test1.exe -r:Claro.dll -r:Mono.Cairo

run: test1
	LD_LIBRARY_PATH=../../src/claro/base:../../src/claro/graphics mono test1.exe

clean:
	rm -f src/*~
	rm -f test/*~
	rm -f Claro.dll
	rm -f test1.exe
