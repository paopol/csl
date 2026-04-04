CC := gcc
CFLAGS := -Wall -Wextra

test: test.c csl.h
	$(CC) $(CFLAGS) $< -o $@