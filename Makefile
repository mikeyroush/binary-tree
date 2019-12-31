all: Main

Main: BTree.h main.cpp
	g++ -std=c++11 main.cpp -o Main

clean:
	rm *.o Main
