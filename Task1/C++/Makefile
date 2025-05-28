CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++20 -O3

PROG = program
OBJECTS = main.o library.o

CLEAN = rm -f $(OBJECTS) $(PROG)

all: $(PROG)

$(PROG): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(PROG)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./$(PROG)

clean:
	$(CLEAN)

test: $(PROG) run
	$(CLEAN)
