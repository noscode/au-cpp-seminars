BIN_DIR=bin/
SRC_DIR=src/
CPP_FLAGS=-Wall -Werror -std=c++11 -ggdb

$(BIN_DIR)/main: $(SRC_DIR)/main.cpp $(SRC_DIR)/thread_safe_obj.h
	g++ $(CPP_FLAGS) $< -o $@
