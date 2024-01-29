objectFolder = bin/obj
buildFolder = bin/build
includeFolder = include 
compileCommand = g++ -Wall -fPIC -c

build_lib : context.o shunting.o newton.o
	g++ -shared -o $(buildFolder)/libnexsys.so $(objectFolder)/context.o $(objectFolder)/shunting.o $(objectFolder)/newton.o

context.o :
	$(compileCommand) src/context.cpp -I $(includeFolder) -o $(objectFolder)/context.o

shunting.o :
	$(compileCommand) src/shunting.cpp -I $(includeFolder) -o $(objectFolder)/shunting.o

newton.o :
	$(compileCommand) src/newton.cpp -I $(includeFolder) -o $(objectFolder)/newton.o