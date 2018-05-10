SRC_DIR = ./src
TEST_DIR = ./tests
ROOT_DIR:=$(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

SHARED_LIB = libdheap.so

TEST_FILES = test buffer_overflow buffer_underflow double_free child execve

CC = gcc
CFLAGS = -shared -Bsymbolic -Wl,--no-as-needed -ldl -fPIC -Wall -Werror

all: library tests

library:
	$(CC) $(CFLAGS) $(SRC_DIR)/*.c -o ${SHARED_LIB}

tests: $(TEST_FILES)

%: $(TEST_DIR)/%.c
	$(CC) -o $@ $<

runtests: tests
	for file in $(TEST_FILES); do \
	  echo $$file; \
		./$$file; \
	done

rundtests: tests library
	for file in $(TEST_FILES); do \
	  echo $$file; \
		LD_PRELOAD=$(ROOT_DIR)/${SHARED_LIB} LIBDHEAP_EXIT_ON_CLOSE=1 ./$$file; \
	done

clean:
	rm $(SHARED_LIB)
	for file in $(TEST_FILES); do \
		rm $$file; \
	done
