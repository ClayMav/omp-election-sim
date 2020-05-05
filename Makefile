CC=g++-9
CFLAGS=-fopenmp -std=c++11

default: 
	$(CC) $(CFLAGS) *.cpp -o electionsim
