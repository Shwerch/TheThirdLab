CXX = g++
CXXFLAGS = -Wall -Wextra -Werror -pedantic -std=c++17 -O3 -Iinclude

SRC_FILES = $(shell find src -type f -name '*.cpp')
OBJECT_FILES = $(SRC_FILES:src/%.cpp=build/%.o)

PROG = program

all: $(PROG)

$(PROG): $(OBJECT_FILES)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJECT_FILES) -o bin/$(PROG)

build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

run:
	./bin/$(PROG)

clean:
	rm -rf build bin
