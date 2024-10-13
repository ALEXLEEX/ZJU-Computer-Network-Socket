# Description: Makefile for server.
# Assemble: make all
# Clean: make clean
# Test: make test
# Author: lqy
# Date: 2024-10-13

# Variables

CXX = g++
CXXFLAGS = -Wall -g -std=c++11 -Iinclude

SRC = src
OD = obj
OBJS = $(patsubst $(SRC)/%.cpp, $(OD)/%.o, $(wildcard $(SRC)/*.cpp))

TARGET = server
RM = rm -f

# Rules

.PHONY: all clean

$(shell mkdir -p $(OD))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OD)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OD)/*.o $(BIN) $(TARGET)