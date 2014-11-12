CPPFLAGS=-Wall -std=c++11 -Werror -ggdb
SRC_DIR=src/
BIN_DIR=bin/

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(SRC_DIR)/main.cpp \
	$(SRC_DIR)/scoped_ptr.h $(SRC_DIR)/noncopyable.h \
	$(SRC_DIR)/pimpl_example.h $(SRC_DIR)/pimpl_example.cpp\
	$(SRC_DIR)/shared_ptr.h
	g++ -o $@ $(CPPFLAGS) $(SRC_DIR)/main.cpp $(SRC_DIR)/pimpl_example.cpp

clean:
	rm -rf $(BIN_DIR)/main

.PHONY: main
