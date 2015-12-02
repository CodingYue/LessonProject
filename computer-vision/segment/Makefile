INCDIR = -I./src/
OPT    = -O3
CPP    = g++
CFLAGS = $(OPT) $(INCDIR)
LINK   = -lm 

.cpp.o:
	$(CPP) $(CFLAGS) -c $< -o $@

all: segment

segment: src/segment.cpp src/segment-image.h src/segment-graph.h src/disjoint-set.h
	$(CPP) $(CFLAGS) -o segment src/segment.cpp $(LINK)

clean:
	/bin/rm -f segment *.o

clean-all: clean
	/bin/rm -f *~ 



