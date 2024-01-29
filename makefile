# Relative filepath to object (.o) files
objectFolder = bin/obj

# Relative filepath to binary output folder (.dll's/.so's/.exe's)
buildFolder = bin/build

# Relative filepath to dir containing headers
includeFolder = include 

build_lib : context.o shunting.o
	g++ -shared -o $(buildFolder)/libnexsys.so $(objectFolder)/context.o $(objectFolder)/shunting.o

context.o :
	g++ -Wall -fPIC -c src/context.cpp -I $(includeFolder) -o $(objectFolder)/context.o

shunting.o :
	g++ -Wall -fPIC -c src/shunting.cpp -I $(includeFolder) -o $(objectFolder)/shunting.o