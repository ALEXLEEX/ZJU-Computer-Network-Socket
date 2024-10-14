# Description: Makefile for server.
# Author: lqy
# Date: 2024-10-13

# Compiler and flags.
CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Iinclude

# Source files and object files.
SRC = src
SRCS = $(wildcard $(SRC)/*.cpp) main.cpp
OD = obj
OBJS = $(patsubst %.cpp, $(OD)/%.o, $(notdir $(SRCS)))

# Target executable.
BIN = bin
TARGET = $(BIN)/server

# Tests.
TEST = test
TESTCASE = $(BIN)/client

# Utilities.
RM = rm -rf

# Build target.
all: $(TARGET)

$(TARGET): $(OBJS) | $(BIN)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OD)/%.o: $(SRC)/%.cpp | $(OD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OD)/main.o: main.cpp | $(OD)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OD):
	mkdir -p $(OD)

$(BIN):
	mkdir -p $(BIN)

# Test cases.
test: $(TESTCASE)

$(TESTCASE): $(TEST)/client.cpp | $(BIN)
	$(CXX) $(CXXFLAGS) $< -o $@

# Clean.
.PHONY: clean
clean:
	$(RM) $(OD) $(BIN)