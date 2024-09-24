# Compiler and flags
CXX = clang++
CXXFLAGS += -fPIC
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
$(EXECUTABLE): $(OBJ_FILES) example_fuzzer.c
	$(CXX) $(OBJ_FILES) example_fuzzer.c -o $(EXECUTABLE) $(CXXFLAGS) -fsanitize=fuzzer

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
