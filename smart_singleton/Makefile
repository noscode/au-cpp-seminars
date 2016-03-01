CPPFLAGS=-Wall -std=c++11 -Werror -ggdb
SRC_DIR=src/
BIN_DIR=bin/

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(SRC_DIR)/main.cpp \
	$(SRC_DIR)/singleton.h
	g++ -o $@ $(CPPFLAGS) $(SRC_DIR)/main.cpp

clean:
	rm -rf $(BIN_DIR)/main

.PHONY: main
