CC = g++

OBJS = parser_tests.o parser.o

tests: $(OBJS) 
	$(CC) -o tests $(OBJS) 

parser_tests.o: parser_tests.cpp
	$(CC) -c parser_tests.cpp

parser.o: ../src/parser.cpp
	$(CC) -c ../src/parser.cpp

clean:
	-rm *.o $(OBJS)
