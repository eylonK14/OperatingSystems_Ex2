$CC = gcc -std=gnuXX
$CFLAGS = -Wall -g

all: ttt.c mync.c
	$(CC) $(CFLAGS) -o ttt ttt.c -lc
	$(CC) $(CFLAGS) -o mync mync.c -lc

clean:
	rm -f ttt mync

.PHONY: clean all
