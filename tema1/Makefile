CFLAGS = -Wall -Werror
CC = gcc
EXEC = tema1

SRCS = main.c functiiLista.c
OBJS = $(SRCS:.c=.o)

build: $(OBJS)
	$(CC) -g $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	./$(EXEC)

clean:
	rm -f $(EXEC) $(OBJS)
