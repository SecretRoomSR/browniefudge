BUILD_DIR = out
SRC_DIR = src
CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall

ifeq ($(OS),Windows_NT)
    EXEEXT := .exe
else
    EXEEXT := 
endif
SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/preference.cpp $(SRC_DIR)/interpreter.cpp
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = $(BUILD_DIR)/browniefudge$(EXEEXT)

.PHONY: all clean build rebuild

all: build

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	@rm -rf $(BUILD_DIR)

build: $(TARGET)

rebuild: clean build
