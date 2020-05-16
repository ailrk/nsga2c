C = gcc
CFLAGS = -Wall -O2
LIBS = -lm

OBJS = utils.o evolution.o fastnondominsort.o crowddist.o

all: nsga2

nsga2.so: nsga2.o
	$(CC) -shared nsga2.o

nsga2.o: $(OBJS) defs.h nsga2.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -fPIC -c -o nsga2.o

evolution.o: evolution.h defs.h fastnondominsort.h crowddist.h
crowddist.o: crowddist.h defs.h utils.h
fastnondominsort.o: fastnondominsort.h defs.h utils.h
utils.o: utils.h defs.h

.PHONY: clean
clean:
	@rm $(OBJS)
