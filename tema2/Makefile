CFLAGS = -Wall -Werror
CC = gcc
EXEC = tema2

SRCS = main.c functiiAux.c
OBJS = $(SRCS:.c=.o)

build: $(OBJS)
	$(CC) -g $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) $(OBJS)
