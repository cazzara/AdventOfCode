# Default compiler
CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++11 -Iinclude

# Ensure arguments are provided
ifeq ($(YEAR),)
  $(error Usage: make YEAR=2024 DAY=day5 PART=part1)
endif
ifeq ($(DAY),)
  $(error Usage: make YEAR=2024 DAY=day5 PART=part1)
endif
ifeq ($(PART),)
  $(error Usage: make YEAR=2024 DAY=day5 PART=part1)
endif

# Source and output file paths
SRCDIR := $(YEAR)/$(DAY)/$(PART)
SRC := $(wildcard $(SRCDIR)/*.cpp) include/utils.cpp
OBJ := $(SRC:.cpp=.o)
EXE := $(SRCDIR)/solution

# Default rule
all: $(EXE)

# Linking
$(EXE): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compiling
$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean rule
clean:
	rm -f $(SRCDIR)/*.o include/utils.o $(EXE)

# Prevent make from treating the arguments as files
.PHONY: all clean