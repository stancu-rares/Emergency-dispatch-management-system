EXEC = tema1

CC = gcc

CFLAGS = -Wall -Wextra -g

SRCS = main.c struct.c

build: $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(EXEC)

clean:
	rm -f $(EXEC) *.o