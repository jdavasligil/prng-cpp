# Name of output binary executable
BINARY=bin

# List of code (.cpp) directories (sep=space)
CODEDIRS=src

# List of include (.h) directories (sep=space)
INCDIRS=./src

# Location to store binary
BUILD=build

# Compiler (g++/clang++)
CC=clang++

# Optimization flags (https://clang.llvm.org/docs/CommandGuide/clang.html)
OPT=-Og

# Standard Supported
STD=-std=c++17

# Encode make rules for .h dependencies
DEPFLAGS=-MP -MD

# Extra flags (https://gcc.gnu.org/onlinedocs/gcc/Option-Summary.html)
CFLAGS=-Wall -Wextra -g $(foreach D,$(INCDIRS),-I$(D)) $(OPT) $(DEPFLAGS) $(STD)

# List all .cpp files using regex (wildcard) pattern matching.
CFILES=$(foreach D,$(CODEDIRS),$(wildcard $(D)/*.cpp))

OBJECTS=$(patsubst %.cpp,%.o,$(CFILES))
DEPFILES=$(patsubst %.cpp,%.d,$(CFILES))

all: $(BUILD)/$(BINARY)

run: $(BUILD)/$(BINARY)
	./$^

debug: $(BUILD)/$(BINARY)
	gdb ./$^

$(BUILD)/$(BINARY): $(OBJECTS)
	$(CC) -o $@ $^

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<

clean: 
	rm -rf $(BINARY) $(OBJECTS) $(DEPFILES)

pack: clean
	tar zcvf pkg.tgz *

diff:
	$(info Repository status and volume of per-file changes:)
	@git status
	@git --no-pager diff --stat
