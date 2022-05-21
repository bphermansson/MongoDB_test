########################################################################
####################### Makefile Template ##############################
########################################################################

# Compiler settings - Can be customized.
CC = gcc
CFLAGS = -Wall -I./lib/ping -I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -lmongoc-1.0 -lbson-1.0 -ljansson
#CXXFLAGS = -g -O0 -Wextra -Wshadow -pedantic -Ilib -Ilib/ping -I/usr/include/libbson-1.0 -I/usr/include/libmongoc-1.0 -lmongoc-1.0 -lbson-1.0 
OBJ = lib/ping/ping.c lib/list_posts/list_posts.c src/main.c
objects = obj/*

LDFLAGS = 
LDLIBS =


%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

mongodemo: $(OBJ)
	gcc $(CFLAGS) -o $@ $^

.PHONY : clean
clean :
	-rm mongodemo $(objects)