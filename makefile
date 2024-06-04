# $CC = gcc
# $CFLAGS = -Wall -g


# all: ttt mync libparse.a
# 	$(CC) $(CFLAGS) -o ttt ttt.c -lc
# 	$(CC) $(CFLAGS) -o mync mync.c -lc

# ttt: ttt.c
# 	$(CC) $(CFLAGS) -o ttt ttt.c -lc

# mync: mync.o libparse.a
# 	$(CC) $(CFLAGS) -o mync mync.o -L. -lparse

# mync.o: mync.c
# 	$(CC) $(CFLAGS) -c mync.c -o mync.o

# parser.o: parser.c parser.h
# 	$(CC) $(CFLAGS) -c parser.c -o parser.o

# libparse.a: udpHandler.o tcpHandler.o parser.o
# 	ar rcs libparse.a udpHandler.o tcpHandler.o parser.o

# udpHandler.o: udpHandler.c udpHandler.h
# 	$(CC) $(CFLAGS) -c udpHandler.c -o udpHandler.o

# tcpHandler.o: tcpHandler.c tcpHandler.h
# 	$(CC) $(CFLAGS) -c tcpHandler.c -o tcpHandler.o


# clean:
# 	rm -f *.o *.a ttt mync

# .PHONY: clean all

# Variables
CC = gcc
CFLAGS = -Wall -Wextra -Werror
LDFLAGS = -static
LIBNAME = libmylib.a

# Object files for the library
OBJ_LIB_FILES = parser.o tcpHandler.o udpHandler.o

# Object files for executables
OBJ_EXEC_FILES = ttt.o mync.o

# Static library
$(LIBNAME): $(OBJ_LIB_FILES)
	ar rcs $(LIBNAME) $(OBJ_LIB_FILES)
	ranlib $(LIBNAME)

# Compile each.c file into an object file (.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule for creating the static library
lib: $(LIBNAME)

# Rule for creating executables
executables: $(OBJ_EXEC_FILES)
	$(CC) $(CFLAGS) -o ttt ttt.o
	$(CC) $(CFLAGS) -o mync mync.o $(LIBNAME)

# Default target
all: lib executables

# Clean up
clean:
	rm -f $(OBJ_LIB_FILES) $(OBJ_EXEC_FILES) $(LIBNAME) ttt mync

.PHONY: clean all