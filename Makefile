CC = gcc
CFLAGS = -Wall -Werror
DEPS = functii.h
OBJ = main.o functii.o

build: tema1 

tema1: $(OBJ) $(DEPS)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

run:
	./tema1

.PHONY: clean
clean:
	rm *.o tema1