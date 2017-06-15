SRC_DIR = ./src
TEST_DIR = ./test
BUILD_DIR = ./build
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

SRC_FILES = libdheap.c
SHARED_LIB = libdheap.so

CC = gcc
CFLAGS = -shared -Bsymbolic -Wl,--no-as-needed -ldl -fPIC -Wall -Werror

dir_guard = @mkdir -p $(@D)

all: library test

library:
	$(dir_guard)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(SRC_FILES) -o $(BUILD_DIR)/${SHARED_LIB}

test: library
	$(dir_guard)
	$(CC) -o $(BUILD_DIR)/test $(TEST_DIR)/test.c

runtest: test
	$(BUILD_DIR)/test

runctest: test library
	LD_PRELOAD=$(ROOT_DIR)/$(BUILD_DIR)/${SHARED_LIB} $(BUILD_DIR)/test