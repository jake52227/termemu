CC = g++
OBJS = text_container.o text_container_tests.o test_runner.o

text_container_tests: $(OBJS)
	$(CC) -o text_container_tests $(OBJS)

text_container_tests.o: text_container_tests.cpp
	$(CC) -c text_container_tests.cpp

text_container.o: ../../src/text_container.cpp
	$(CC) -c ../../src/text_container.cpp

test_runner.o: ../test_runner.cpp
	$(CC) -c ../test_runner.cpp