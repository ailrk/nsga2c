C = gcc
CFLAGS = -Wall -O2
LIBS = -lm

OBJS = utils.o evolution.o

all: nsga2

nsga2: $(OBJS) defs.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o nsga2

evolution.o: evolution.c evolution.h defs.h

utils.o: utils.c utils.h defs.h

clean:
	@rm ./*.o
