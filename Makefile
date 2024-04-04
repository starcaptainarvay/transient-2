CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lvulkan -lglfw

SRC_DIR = src
BUILD_DIR = build
TARGET = Transient.exe

SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

.PHONY: all clean

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)
	$(RM) -r build/main.o

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	$(RM) -r $(BUILD_DIR)