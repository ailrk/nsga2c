C = gcc
CFLAGS = -Wall -O2
LIBS = -lm

OBJS = utils.o evolution.o fastnondominsort.o crowddist.o

all: nsga2

nsga2: $(OBJS) defs.h nsga2.h
	$(CC) $(CFLAGS) $(OBJS) $(LIBS) -o nsga2

evolution.o: evolution.c evolution.h defs.h fastnondominsort.h \
	crowddist.h

crowddist.o: crowddist.c crowddist.h defs.h utils.h

fastnondominsort.o: fastnondominsort.c fastnondominsort.h defs.h utils.h

utils.o: utils.c utils.h defs.h

clean:
	@rm ./*.o
