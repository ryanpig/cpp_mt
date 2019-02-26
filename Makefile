# The name of the final executable
TARGET := basic_thread condition_variable mutex profiling_async
# \mutex condition_variable 
# The directory in which to put compilation artifacts

# Compiler choice and base flags
CXX := g++ 
CXXFLAGS := -Wall -Wextra -std=gnu++11 -pthread
LDFLAGS :=

# Disable optimisation in debug mode.
# To enable debug mode, pass e.g. DEBUG=1 as an extra argument to 'make'.
ifeq ($(DEBUG),)
	CXXFLAGS += -O3 
	# CXXFLAGS += -fsanitize=address 
endif

# Detect platform and enable specific features.
UNAME_VAL := $(shell uname)
ifeq ($(UNAME_VAL),Darwin)
	CXXFLAGS += -DHAVE_SIGACTION -flto
else ifeq ($(UNAME_VAL),Linux)
	CXXFLAGS += -DHAVE_SIGACTION
endif

# Collect sources
CXX_SOURCES := $(wildcard *.cpp)

# Compute necessary compilation artifacts
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJDIR)/%.o,$(CXX_SOURCES))

# Generic targets
.PHONY: all clean

all: $(TARGET)


# cleaning after building
clean:
	rm -f $(TARGET)
	rm -f *.o

$(TARGET): $(CXX_SOURCES) iprof/iprof.cpp
	@echo LD $@ 
	@$(CXX) $(CXXFLAGS) -o $@ $@.cpp iprof/iprof.cpp $(LDFLAGS)

# Make sure the dependency relations get read by 'make'
-include $(DEP_FILES)
