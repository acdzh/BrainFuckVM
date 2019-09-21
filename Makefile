CC=g++
SRCS=./src/bl.cc
OBJS=$(SRCS:.c=.o)
EXEC=./release/bl

start:$(OBJS)
	$(CC) -o $(EXEC) $(OBJS)
.c.o:
	$(CC) -o $@ -c $< -fPIC
clean:
	rm -rf $(OBJS)