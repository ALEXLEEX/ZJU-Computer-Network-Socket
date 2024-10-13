CXX = g++
INCLUDE = include
CXXFLAGS = -Wall -g -std=c++11 -I$(INCLUDE)
SRC = src
OD = obj
OBJS = $(patsubst $(SRC)/%.cpp, $(OD)/%.o, $(wildcard $(SRC)/*.cpp))
TARGET = server
RM = rm -f

.PHONY: all clean

$(shell mkdir -p $(OD))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(OD)/%.o: $(SRC)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	$(RM) $(OD)/*.o $(TARGET) test/*