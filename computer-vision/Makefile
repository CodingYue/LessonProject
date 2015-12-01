CXX = g++

SOURCES = main.cpp
OBJS = $(SOURCES:.cpp=.o)

CXXFLAGS = -O2 `pkg-config --cflags opencv`
LDFLAGS = `pkg-config --libs opencv`

.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $^

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

all: main

clean:
	rm -rf *.o main
