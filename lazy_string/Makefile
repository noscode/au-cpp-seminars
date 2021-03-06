OUT_DIR=bin/
SRC_DIR=src/
TEST_DIR=test/
CPP_FLAGS=-Wall -Werror -ggdb --std=c++11

cpp_files=$(filter %.cpp, $1)
obj_files=$(filter %.o, $1)
file_name=$(notdir $1)

# ============ TESTS +==============
all: tests

tests: $(OUT_DIR)/test $(OUT_DIR)/run_test.sh

$(OUT_DIR)/run_test.sh:
	cp $(TEST_DIR)/$(call file_name,$@) $@
	chmod +x $@

$(OUT_DIR)/test: $(TEST_DIR)/test.cpp $(SRC_DIR)/lazy_string_utils.h \
	$(OUT_DIR)/lazy_string.o $(OUT_DIR)/shared_buffer.o \
	$(OUT_DIR)/lazy_string_ix_modifier.o
	g++ -I $(SRC_DIR) $(call cpp_files,$^) $(call obj_files,$^) $(CPP_FLAGS) -o $@

# ========= LAZY STRING ============

$(OUT_DIR)/lazy_string.o: $(SRC_DIR)/lazy_string.h $(SRC_DIR)/lazy_string.cpp \
	$(SRC_DIR)/lazy_string_ix_modifier.h
	g++ $(call cpp_files,$^) $(CPP_FLAGS) -c -o $@

$(OUT_DIR)/lazy_string_ix_modifier.o: \
	$(SRC_DIR)/lazy_string_ix_modifier.h $(SRC_DIR)/lazy_string_ix_modifier.cpp \
	$(SRC_DIR)/lazy_string.h
	g++ $(call cpp_files,$^) $(CPP_FLAGS) -c -o $@

$(OUT_DIR)/shared_buffer.o: $(SRC_DIR)/shared_buffer.cpp $(SRC_DIR)/shared_buffer.h
	g++ $(call cpp_files,$^) $(CPP_FLAGS) -c -o $@

# ============== COMMON ============
clean:
	rm -rf $(OUT_DIR)/*

.PHONY: tests clean
