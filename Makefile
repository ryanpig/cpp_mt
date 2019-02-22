# The name of the final executable
TARGET := basic_thread condition_variable mutex
# \mutex condition_variable 
# The directory in which to put compilation artifacts
OBJDIR := .objs
# SRC_DIR = . 

# Compiler choice and base flags
CXX := g++ -fdiagnostics-color=always
CXXFLAGS := -Wall -Wextra -std=c++14 -g -ffast-math -march=native -mtune=native -pthread
LDFLAGS :=

# Disable optimisation in debug mode.
# To enable debug mode, pass e.g. DEBUG=1 as an extra argument to 'make'.
ifeq ($(DEBUG),)
	CXXFLAGS += -O3 
	CXXFLAGS += -fsanitize=address 
endif

# Detect platform and enable specific features.
UNAME_VAL := $(shell uname)
ifeq ($(UNAME_VAL),Darwin)
	CXXFLAGS += -DHAVE_SIGACTION -flto
else ifeq ($(UNAME_VAL),Linux)
	CXXFLAGS += -DHAVE_SIGACTION
endif

# BOOST program_options library 
# ifeq ($(UNAME_VAL),Darwin)
#   BOOST_ROOT = /usr/local/opt/boost
#   BOOST_CXXFLAGS := -I${BOOST_ROOT}/include
#   BOOST_LDFLAGS := -L${BOOST_ROOT}/lib -lboost_program_options
# else ifeq ($(UNAME_VAL),Linux)
#   BOOST_ROOT = $(shell ls -d ~/miscellaneous/boost_* | tail -1)
#   BOOST_ROOT = ~/miscellaneous/boost_1_69_0
#   BOOST_CXXFLAGS := -I${BOOST_ROOT}/boost
#   BOOST_LDFLAGS := -L${BOOST_ROOT}/stage/lib -lboost_program_options
# endif

# CXXFLAGS += $(BOOST_CXXFLAGS)
# LDFLAGS += $(BOOST_LDFLAGS)

# # sqlite 3
# SQLITE3_LDFLAGS := -lsqlite3
# LDFLAGS += $(SQLITE3_LDFLAGS)

# Flags for testing
CXXFLAGS_TEST := $(CXXFLAGS)
LDFLAGS_TEST  := $(LDFLAGS)

# Collect sources
SRC_DIR_TEST = . 
CXX_SOURCES := $(wildcard *.cpp)

# Compute necessary compilation artifacts
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJDIR)/%.o,$(CXX_SOURCES))
# DEP_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJDIR)/%.d,$(CXX_SOURCES))

# Generic targets
.PHONY: all clean

all: $(TARGET)


# cleaning after building
clean:
	rm -f $(TARGET)
	rm -f *.o

$(TARGET): $(CXX_SOURCES)
	@echo LD $@ 
	@$(CXX) $(CXXFLAGS) -o $@ $@.cpp $(LDFLAGS)

# @$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
# $(OBJDIR)/%.o: $(SRC_DIR)/%.cpp
#   @mkdir -p $(dir $@)
#   @echo CXX $<
#   @$(CXX) $(CXXFLAGS) -c -o $@ $<

# $(OBJDIR)/%.d: $(SRC_DIR)/%.cpp
#   @mkdir -p $(dir $@)
#   @echo DEP $<
#   @$(CXX) -MT $(OBJDIR)/$*.o -MM $(CXXFLAGS) $< >$@

# Make sure the dependency relations get read by 'make'
-include $(DEP_FILES)
