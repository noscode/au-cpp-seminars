all: ./bin/main ./bin/class_tests

./bin/main: ./src/main.cpp ./src/event.h
	g++ -Wall -Werror -std=c++11 -g $< -o $@

./bin/class_tests: ./src/class_tests.cpp ./src/event.h
	g++ -Wall -Werror -std=c++11 -g $< -o $@

.PHONY: all clean

clean:
	rm -rf ./bin/*
