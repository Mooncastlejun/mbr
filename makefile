# Compiler and flags
CXX = clang++
CXXFLAGS += -fPIC -g0  # -g0 플래그 추가
LDFLAGS = -shared

# Object files
OBJ_FILES = mbr.o

# Static and shared library names
STATIC_LIB = libmbr.a
SHARED_LIB = libmbr.so

# Executable name
EXECUTABLE = mbr_test

# Default target
all: $(EXECUTABLE) $(STATIC_LIB) $(SHARED_LIB)

# Compile the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $(EXECUTABLE) $(CXXFLAGS)

# Create static library
$(STATIC_LIB): $(OBJ_FILES)
	ar rcs $@ $^

# Create shared library
$(SHARED_LIB): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile object files
mbr.o: mbr.c mbr.h
	$(CXX) $(CXXFLAGS) -c mbr.c -o mbr.o

# Clean up
clean:
	rm -f $(EXECUTABLE) $(OBJ_FILES) $(STATIC_LIB) $(SHARED_LIB)

.PHONY: all clean 
