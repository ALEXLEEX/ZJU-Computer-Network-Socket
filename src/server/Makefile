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

# Utilities.
RM = rm -f

# Rules
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

.PHONY: clean
clean:
	$(RM) $(OD)/*.o $(TARGET)