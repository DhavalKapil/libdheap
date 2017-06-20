SRC_DIR = ./src
TEST_DIR = ./tests
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

SHARED_LIB = libdheap.so

CC = gcc
CFLAGS = -shared -Bsymbolic -Wl,--no-as-needed -ldl -fPIC -Wall -Werror

all: library test

library:
	$(CC) $(CFLAGS) $(SRC_DIR)/*.c -o ${SHARED_LIB}

test: library
	$(CC) -o test $(TEST_DIR)/test.c

runtest: test
	./test

runctest: test library
	LD_PRELOAD=$(ROOT_DIR)/${SHARED_LIB} ./test