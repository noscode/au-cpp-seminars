BIN_DIR=./bin/
SRC_DIR=./src/
INCLUDE_DIR=./include/
LIB_NAME=libpthread_msanity
LIB_OBJ=$(BIN_DIR)/$(LIB_NAME).o
LIB_SHARED=$(BIN_DIR)/$(LIB_NAME).so
LIB_STATIC=$(BIN_DIR)/$(LIB_NAME).a
TEST_BIN=$(BIN_DIR)/test

INCLUDES=-I$(INCLUDE_DIR) -I../mutex_module/
CFLAGS=-std=c++11 -Wall -Werror -fPIC
LIBS=-ldl

all: $(LIB_SHARED) $(LIB_STATIC) $(TEST_BIN)

$(LIB_STATIC): $(LIB_OBJ)
	ar rcs $@ $<

$(LIB_SHARED): $(LIB_OBJ)
	g++ -shared $< -o $@ $(LIBS)

$(LIB_OBJ): $(SRC_DIR)/pthread_msanity.cpp \
	$(INCLUDE_DIR)/pthread_msanity.h
	g++ -c $<  $(CFLAGS) $(INCLUDES) -o $@ $(LIBS)

$(TEST_BIN): $(SRC_DIR)/test.cpp $(LIB_STATIC) $(LIB_SHARED)
	g++ $< $(CFLAGS) -pthread -o $@ $(LIBS)

clean:
	    rm -rf $(BIN_DIR)/*

.PHONY: clean
