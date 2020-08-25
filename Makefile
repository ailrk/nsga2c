C = clang
CFLAGS = -Wall -O2
LIBS = -lm

OBJS = utils.o evolution.o fastnondominsort.o crowddist.o

nsga2.o $(OBJS):
	ld -relocatable $(OBJS) -o $@

evolution.o: evolution.c evolution.h defs.h utils.h crowddist.h \
 fastnondominsort.h
crowddist.o: crowddist.c crowddist.h defs.h utils.h
fastnondominsort.o: fastnondominsort.c fastnondominsort.h defs.h utils.h
utils.o: utils.c utils.h defs.h crowddist.h

.PHONY: clean
clean:
	@rm $(OBJS)
