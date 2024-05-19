$CC = gcc
$CFLAGS = -Wall -g

all: ttt.c
	$(CC) $(CFLAGS) -o ttt ttt.c

clean:
	rm -f ttt

.PHONY: clean all
