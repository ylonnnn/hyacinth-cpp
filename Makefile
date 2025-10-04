LINUX_CXX := g++
WIN_CXX := x86_64-w64-mingw32-g++

CXXFLAGS := -ggdb -O0 -Wall -Wextra -std=c++17 -Iinclude -MMD -MP
LDFLAGS := -fsanitize=address

WIN_LDFLAGS = -static

BUILD_DIR := $(CURDIR)/build
SRC_DIR := ./src

OBJ_DIR := ./obj
LINUX_OBJ_DIR := $(OBJ_DIR)/linux
WIN_OBJ_DIR := $(OBJ_DIR)/win

SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
LINUX_OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(LINUX_OBJ_DIR)/%.o)
WIN_OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(WIN_OBJ_DIR)/%.o)

LINUX_DEPS := $(LINUX_OBJECTS:.o=.d)
WIN_DEPS := $(WIN_OBJECTS:.o=.d)

LINUX_TARGET := $(BUILD_DIR)/hyc
WIN_TARGET := $(BUILD_DIR)/hyc.exe

.PHONY: all clean

all: linux windows

linux: $(LINUX_TARGET)

windows: $(WIN_TARGET)

$(LINUX_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(LINUX_CXX) $(CXXFLAGS) -c $< -o $@

$(WIN_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(WIN_CXX) $(CXXFLAGS) -c $< -o $@

$(LINUX_TARGET): $(LINUX_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(LINUX_CXX) $(LINUX_OBJECTS) -o $(LINUX_TARGET)
	# $(LINUX_CXX) $(LINUX_OBJECTS) $(LDFLAGS) -o $(LINUX_TARGET)

$(WIN_TARGET): $(WIN_OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(WIN_CXX) $(WIN_OBJECTS) $(WIN_LDFLAGS) -o $(WIN_TARGET)
	# $(WIN_CXX) $(WIN_OBJECTS) $(LDFLAGS) $(WIN_LDFLAGS) -o $(WIN_TARGET)

lr: linux
	$(LINUX_TARGET)

wr: windows
	$(WIN_TARGET)

vars:
	@echo "Sources: $(SOURCES)"
	@echo "Linux Objects: $(LINUX_OBJECTS)"
	@echo "Windows Objects: $(WIN_OBJECTS)"

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)


-include $(LINUX_DEPS)
-include $(WIN_DEPS)
