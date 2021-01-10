## Tools
CXX = g++
DISS = objdump

## Flags
CXX_FLAGS = -Wall -g $(UVAR)

## Source files
SRCS = src/*.cpp
SRCS += tests/$(TEST).cpp

## Object files
OBJS = *.o

## Include directories
INC_DIRS = -Iinc/

## Default target
all: clean compile link clean_obj run

## Link target
link:
	$(CXX) $(CXX_FLAGS) -o build/bin_$(TEST) $(OBJS)

## Compile target
compile:
	$(CXX) $(CXX_FLAGS) -c $(SRCS) $(INC_DIRS)

## Clean target
clean:
	rm -rf *.o build/*

## Clean only compile object
clean_obj:
	rm -rf *.o

## Run test
run:
	./build/bin_$(TEST)

help:
	@echo "make clean - deletes the ./*.o and binaries in ./build"
	@echo "make TEST=xyz - cleans, compiles, links and runs test xyz"
