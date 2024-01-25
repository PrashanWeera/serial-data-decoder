# Makefile for a simple C++ program

# Compiler and compiler flags
CXX = g++
CXXFLAGS = -std=c++14 -Wall -g

# Target executables
TARGET = DataPasser
TEST_TARGET = Tester

TARGET_DIR = build

# Source files
SOURCES = DataPasserMain.cpp SerialReader.cpp DataCollector.cpp DataPasser.cpp

# Test source files
TEST_SOURCES = Tester.cpp SerialReader.cpp DataCollector.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o)

# Include directories
INCLUDE_DIRS = -I./include -I../json/include/

# Libraries
LIBS = -lgtest -lgtest_main

# Default target
all: $(TARGET)
	mkdir -p $(TARGET_DIR)
	mv $(TARGET) $(TARGET_DIR)/$(TARGET)
	rm *.o

# Rule to build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o $@ $^ $(LIBS)

# Rule to build the executable
$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -o $@ $^ $(LIBS)

# Rule to build object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) -c -o $@ $<

test: $(TEST_TARGET)
	mkdir -p $(TARGET_DIR)
	mv $(TEST_TARGET) $(TARGET_DIR)/$(TEST_TARGET)
	rm *.o

# Clean target
clean:
	rm -f $(TARGET) $(OBJECTS)
	rm -rf $(TARGET_DIR)
	rm *.o
