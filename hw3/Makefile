# SOURCE FILENAMES
EXECUTABLE = pa3
#
# SYSTEM
CC = gcc
CFLAGS = gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -O3 *.c -o pa3
#---------------------------------------------------------
# RULES
pa3: 
	gcc -std=c99 -pedantic -Wvla -Wall -Wshadow -g *.c -o pa3

test:
	./pa3 examples/4_5.b 4_5t.t 4_5t.f 4_5t.p

test2:
	./pa3 examples/5_4.b 5_4t.t 5_4t.f 5_4t.p

test3:
	./pa3 examples/5_5.b 5_5t.t 5_5t.f 5_5t.p

test4:
	./pa3 examples/7_7.b 7_7t.t 7_7t.f 7_7t.p

.PHONY: pa3 test test2 test3 test4