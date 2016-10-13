CPP_FLAGS=-Wall -Werror --std=c++11 -ggdb

bin/main: ./src/main.cpp
	g++ $(CPP_FLAGS) $< -o $@

clean:
	rm -rf ./bin/*

.PHONY: clean
