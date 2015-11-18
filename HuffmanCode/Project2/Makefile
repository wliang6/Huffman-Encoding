# Makefile

OBJS =bstream.o 
CC = g++
DEBUG = -g
CFLAGS = -Wall -pedantic -ansi $(DEBUG)
LFLAGS = -Wall $(DEBUG)

programs: bstream_driver huffman_test

bstream_driver: bstream.o test.cpp
	  $(CC) $(LFLAGS) test.cpp bstream.o  -o bstream_driver


huffman_test: bstream.o huffman.cpp
	  $(CC) $(LFLAGS) huffman.cpp bstream.o  -o huffman_test

bstream.o: bstream.cpp bstream.h
	$(CC) -c $(CFLAGS) bstream.cpp


clean:
	rm -f *.o huffman_test bstteam_driver 
