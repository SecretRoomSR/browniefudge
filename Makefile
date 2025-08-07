BUILD_DIR = out
CXX = clang++
CXXFLAGS = -std=c++17 -O2 -Wall

.PHONY: all clean build rebuild

all: build

$(BUILD_DIR)/browniefudge.exe: main.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	@rm -rf $(BUILD_DIR)

build: $(BUILD_DIR)/browniefudge.exe

rebuild: clean build
