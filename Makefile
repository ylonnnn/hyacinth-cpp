CXX = g++
CXXFLAGS = -g -O0 -Wall -Wextra -std=c++17 -Iinclude -MMD -MP
# LDFLAGS = -fsanitize=address

BUILD_DIR = $(CURDIR)/build
SRC_DIR = ./src
OBJ_DIR = ./obj

SOURCES := $(shell find $(SRC_DIR) -name "*.cpp")
OBJECTS := $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET := $(BUILD_DIR)/hyc

all: $(TARGET)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(OBJECTS) -o $(TARGET)
	# $(CXX) $(OBJECTS) $(LDFLAGS) -o $(TARGET)

run: 
	$(MAKE) all
	$(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(OBJ_DIR)

.PHONY: all clean

DEPS := $(OBJECTS:.o=.d)
-include $(DEPS)
