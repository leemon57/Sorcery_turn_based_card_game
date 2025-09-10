CXX      := g++-14
CXXFLAGS := -std=c++20 -Wall -MMD -Werror=vla -g

# Where to look for .cc files
SRCDIRS  := . Concrete_Activated Concrete_Triggered Concrete_Spells sorcery-asciiart
# Find all .cc under those dirs
SRCS     := $(foreach d,$(SRCDIRS),$(wildcard $(d)/*.cc))

# Turn foo.cc into foo.o
OBJECTS  := $(SRCS:.cc=.o)

EXEC     := sorcery

all: $(EXEC)

# Link step—now includes ALL object files
$(EXEC): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Compile any .cc into .o
%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include auto‑generated dependency files
-include $(OBJECTS:.o=.d)

clean:
	rm -f $(OBJECTS) $(EXEC) $(OBJECTS:.o=.d)