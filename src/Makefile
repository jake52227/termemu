CC = g++

tests: parser.o tests.o
	$(CC) -o tests parser.o tests.o

parser.o: parser.cpp parser.hpp
	$(CC) -c parser.cpp

tests.o: tests.cpp
	$(CC) -c tests.cpp
