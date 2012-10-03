src=$(wildcard src/*.c)
obj=$(addprefix obj/,$(notdir $(src:.c=.o)))
CC=gcc
CFLAGS=-I. -Wall -std=c99 -lm
all: AD3zip
debug: CFLAGS += -g -DDEBUG
AD3zip: $(obj)
	$(CC) $(CFLAGS) $(obj) -o $@
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	if [ -f "AD3zip" ]; then \
	rm AD3zip; \
	fi; done
	@for file in $(obj); do \
	if [ -f $$file ]; then \
	rm "$$file"; \
	fi; done

