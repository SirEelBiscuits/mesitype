# Generic makefile for C libraries

TARGET=mesitype
#CXX=g++

C_FLAGS+= -std=c++14 --pedantic -w -g

SRC_FILES = $(shell find . -name '*.cpp' | grep -v tee)

all: $(TARGET)

test: $(TARGET)
	@echo "Running tests..."
	@./$(TARGET)
	@echo "Done"

$(TARGET): $(SRC_FILES) ../mesitype.h
	@echo "Building $(TARGET)"
	@$(CXX) $(C_FLAGS) $(SRC_FILES) -o $(TARGET)
	@echo "Done"

clean:
	@echo "Cleaning"
	@rm $(TARGET)
	@echo "Done"

.PHONY: clean 
