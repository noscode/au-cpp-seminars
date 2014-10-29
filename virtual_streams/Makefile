CPP_FLAGS=-O2 -Wall -Werror -ggdb -std=c++11

all: bin/main bin/mainc

bin/main: src/main.cpp \
	src/my_ostream.cpp src/my_ostream.h \
	src/my_ostream_con.h src/my_ostream_con.cpp  \
	src/my_ostream_file.h src/my_ostream_file.cpp
	g++ $(CPP_FLAGS) src/main.cpp src/my_ostream.cpp src/my_ostream_con.cpp src/my_ostream_file.cpp -o bin/main

bin/mainc: srcc/main.cpp \
	srcc/my_ostream.cpp srcc/my_ostream.h \
	srcc/my_ostream_file.h srcc/my_ostream_file.cpp
	g++ $(CPP_FLAGS) srcc/main.cpp srcc/my_ostream.cpp srcc/my_ostream_file.cpp -o bin/mainc

clean:
	rm -rf bin/*

.PHONY: clean
