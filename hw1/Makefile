#
#---------------------------------------------------------
#
# SOURCE FILENAMES
BASE_NAME = shell
EXECUTABLE = pa1
TEST_C = $(EXECUTABLE).c
SRC_C = $(BASE_NAME)_array.c $(BASE_NAME)_list.c 
#
# SYSTEM
SHELL = /bin/bash
CC = gcc
CFLAGS = -std=c99 -Wall -Wshadow -Wvla -pedantic 
#---------------------------------------------------------
# RULES

$(EXECUTABLE):$(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) -O3  $(CFLAGS) $(SRC_C) $(TEST_C) -o $(EXECUTABLE)

test: $(EXECUTABLE)

valgrind_a:
	valgrind ./$(EXECUTABLE) -a 15.b 15sa.b

valgrind_l:
	valgrind ./$(EXECUTABLE) -l 15.b 15sa.b	

.PHONY: test valgrind_a valgrind_l