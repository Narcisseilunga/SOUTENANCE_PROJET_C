
CC = gcc
CFLAGS = -I./include
DEPS = include/database.h include/record.h
OBJ = src/database.o src/record.o main.o

%.o: %.c $(DEPS)
    $(CC) -c -o $@ $< $(CFLAGS)

projet_sgbdr: $(OBJ)
    $(CC) -o $@ $^ $(CFLAGS)

clean:
    rm -f *.o projet_sgbdr
