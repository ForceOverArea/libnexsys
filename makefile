objectFolder = bin/obj
buildFolder = bin/build
includeFolder = include 
testFolder = bin/test

# Build jobs
build_lib : context.o shunting.o newton.o
	g++ -shared -o $(buildFolder)/libnexsys.so $(objectFolder)/*
	rm $(objectFolder)/*

context.o :
	g++ -Wall -fPIC -c src/context.cpp -I $(includeFolder) -o $(objectFolder)/context.o

shunting.o : context.o
	g++ -Wall -fPIC -c src/shunting.cpp -I $(includeFolder) -o $(objectFolder)/shunting.o

newton.o : shunting.o
	g++ -Wall -fPIC -c src/newton.cpp -I $(includeFolder) -o $(objectFolder)/newton.o

# Test jobs
test : test_variable


test_variable :
	g++ -Wall test/test_variable.cpp -I $(includeFolder) -o $(testFolder)/test_variable
	./$(testFolder)/test_variable
	
