src=$(wildcard src/*.c)
obj=$(addprefix obj/,$(notdir $(src:.c=.o)))
CC=gcc
CFLAGS=-I. -Wall -std=c99 -lm
all: AD3zip
debug: CC += -g -DDEBUG -lm
debug: AD3zip
decode_debug: CC += -g -DDECODE_DEBUG -lm
decode_debug: AD3zip

AD3zip: $(obj)
	$(CC) $(CFLAGS) $(obj) -lm -o $@
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -lm -c -o $@ $<
clean:
	if [ -f "AD3zip" ]; then \
	rm AD3zip; \
	fi; done
	@for file in $(obj); do \
	if [ -f $$file ]; then \
	rm "$$file"; \
	fi; done

