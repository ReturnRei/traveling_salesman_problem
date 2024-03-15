#CXX = clang++
CXX = /opt/homebrew/opt/llvm/bin/clang++
CXXFLAGS = -I/opt/homebrew/opt/llvm/include -g -O3 -march=native -flto -Wall -std=c++20 
LDFLAGS = -L/opt/homebrew/opt/llvm/lib/c++ -Wl,-rpath,/opt/homebrew/opt/llvm/lib/c++
SRC = src
OBJ = obj
SRCS = $(wildcard $(SRC)/*.cpp) 
OBJS = $(patsubst $(SRC)/%.cpp, $(OBJ)/%.o, $(SRCS))
HEADER = $(SRC)/c_functions.hpp 
TARGET = $(notdir $(shell basename $(CURDIR)))_$(shell uname)

GUARD_NAME = __C_FUNCTIONS_HPP__
TMP = $(SRC)/tmp.hh

all: clean $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p target
	$(CXX) $(CXXFLAGS) -o target/$@ $(OBJS)

$(OBJ)/%.o: $(SRC)/%.cpp
	@mkdir -p $(OBJ)
	$(CXX) $(CXXFLAGS) -c $< -o $@

#cproto not needed here pragma cool
# cproto: # This is hacky as fuck
# 	@echo "Generating prototypes..."
# 	@cproto $(SRC)/c_functions.cpp > $(TMP) 2>/dev/null
# 	@echo "#ifndef $(GUARD_NAME)" > $(HEADER)
# 	@echo "#define $(GUARD_NAME)" >> $(HEADER)
# 	@echo "" >> $(HEADER)	
# 	@cat $(TMP) >> $(HEADER)
# 	@echo "" >> $(HEADER)
# 	@echo "#endif // $(GUARD_NAME)" >> $(HEADER)
# 	@rm -f $(TMP)

clean:
	rm -rf $(OBJ)/* #target/*
