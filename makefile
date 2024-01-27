main : matrix.o main.o
	g++ -o main bin/matrix.o bin/main.o

main.o :
	g++ -c src/main.cpp -I include -o bin/main.o

# This is a lowest-level dependency
matrix.o : 
	g++ -c src/matrix.cpp -I include -o bin/matrix.o