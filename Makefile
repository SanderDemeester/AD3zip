src=$(wildcard src/*.c)
obj=$(addprefix obj/,$(notdir $(src:.c=.o)))
CC=gcc
CFLAGS=-I -Wall -std=c99 -lm
all: AD3zip
AD3zip: $(obj)
	$(CC) $(CFLAGS) $(obj) -o $@
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
bw : move_to_font
move_to_front: huffman