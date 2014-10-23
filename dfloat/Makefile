CPP_FLAGS=-ggdb -Wall -Werror -std=c++11
CPP_INCLUDES=-I./src/

cpp_files=$(filter %.cpp, $1)

all: bin/test

bin/test: test/main.cpp src/ddouble.cpp src/ddouble.h
	g++  $(call cpp_files,$^) $(CPP_FLAGS) $(CPP_INCLUDES) -o $@

clean:
	rm -rf bin/*

.PHONY: clean
