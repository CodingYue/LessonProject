CXX = g++

SOURCES = src/main.cpp
OBJS = $(SOURCES:.cpp=.o)

CXXFLAGS = -O2 `pkg-config --cflags opencv` -std=c++11 -g
LDFLAGS = `pkg-config --libs opencv`

.o:
	$(CXX) $(CXXFLAGS) -o $@ -c $^

main: $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $(OBJS)

all: main

clean:
	rm -rf src/*.o main
