# TLDR We generate an executable based on the directory name, operating system name and architecture. 
CXX = clang++
C = clang
CXXFLAGS =  -g -O3 -march=native -flto -Wall -std=c++20 
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.c) $(wildcard $(SRC)/*.cpp)
OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
OBJS := $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))
HEADER = $(SRC)/c_functions.h 
TARGET = $(notdir $(shell basename $(CURDIR)))_$(shell uname)_$(shell uname -m)
GUARD_NAME = __C_FUNCTIONS_H__
TMP = $(SRC)/tmp.hh


all: clean cproto $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p target
	$(CXX) $(CXXFLAGS) -o target/$@ $(OBJS)

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(OBJ)
	$(CXX) -c $< -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(C) $(CFLAGS) -c $< -o $@

#cproto not needed here pragma is cool but I still want to fallback to pure C if needed
cproto: # This is hacky as fuck
	@echo "Generating prototypes..."
	@cproto $(SRC)/c_functions.c > $(TMP) 2>/dev/null
	@echo "#ifndef $(GUARD_NAME)" > $(HEADER)
	@echo "#define $(GUARD_NAME)" >> $(HEADER)
	@echo "" >> $(HEADER)	
	@cat $(TMP) >> $(HEADER)
	@echo "" >> $(HEADER)
	@echo "#endif // $(GUARD_NAME)" >> $(HEADER)
	@rm -f $(TMP)

clean:
	rm -rf $(OBJ)/* 
