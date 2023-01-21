CC = g++

OBJS = parser_tests.o parser.o test_runner.o

parser_tests: $(OBJS) 
	$(CC) -o parser_tests $(OBJS) 

parser_tests.o: parser_tests.cpp
	$(CC) -c parser_tests.cpp

parser.o: ../../src/parser.cpp
	$(CC) -c ../../src/parser.cpp

test_runner.o: ../test_runner.cpp
	$(CC) -c ../test_runner.cpp