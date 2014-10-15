OUT_DIR=bin
SRC_DIR=src
CPP_FLAGS=-Wall -Werror -ggdb --std=c++11

cpp_files=$(filter $(SRC_DIR)/%.cpp, $1)
obj_files=$(filter $(OUT_DIR)/%.o, $1)

all: $(OUT_DIR)/main

$(OUT_DIR)/main: $(SRC_DIR)/main.cpp $(OUT_DIR)/lazy_string.o $(OUT_DIR)/shared_buffer.o
	g++ $(call cpp_files,$^) $(call obj_files, $^) $(CPP_FLAGS) -o $@

$(OUT_DIR)/lazy_string.o: $(SRC_DIR)/lazy_string.cpp $(SRC_DIR)/lazy_string.h $(OUT_DIR)/shared_buffer.o
	g++ $(call cpp_files,$^) $(CPP_FLAGS) -c -o $@

$(OUT_DIR)/shared_buffer.o: $(SRC_DIR)/shared_buffer.cpp $(SRC_DIR)/shared_buffer.h
	g++ $(call cpp_files,$^) $(CPP_FLAGS) -c -o $@

clean:
	rm -rf $(OUT_DIR)/*

.PHONY: all clean
