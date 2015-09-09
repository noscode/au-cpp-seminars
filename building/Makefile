all: ./bin/main ./bin/mainc

CPP_FLAGS=-Wall -Werror

./bin/main: ./src/main.cpp ./src/add.cpp ./src/add.hpp
	g++ $(CPP_FLAGS) $(wordlist 1, 2, $^) -o $@

./bin/mainc: ./bin/mainc.o ./bin/add.o
	g++ $(CPP_FLAGS) $(wordlist 1, 2, $^) -o $@

./bin/mainc.o: ./src/mainc.cpp ./src/add.h
	g++ $(CPP_FLAGS) -c $< -o $@

./bin/add.o: ./src/add.c ./src/add.h
	gcc $(CPP_FLAGS) -c $< -o $@

clean:
	rm -rf ./bin/*

.PHONY: all clean
