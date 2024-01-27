# Relative filepath to object (.o) files
objectFolder = bin/obj

# Relative filepath to binary output folder (.dll's/.so's/.exe's)
buildFolder = bin/build

# Relative filepath to dir containing headers
includeFolder = include 

main : main.o
	g++ -o $(buildFolder)/main $(objectFolder)/matrix.o $(objectFolder)/main.o

main.o :
	g++ -Wall -c src/main.cpp -I $(includeFolder) -o $(objectFolder)/main.o