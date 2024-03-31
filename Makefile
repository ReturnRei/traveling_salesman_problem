# TLDR We generate an executable based on the directory name, operating system name and architecture. 
CXX = clang++
CXXFLAGS = -static -std=c++20 -O3 -march=native -flto -Wall 
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
TARGET = $(notdir $(shell basename $(CURDIR)))_$(shell uname)_$(shell uname -m)


all: clean $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p target
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)/* 
