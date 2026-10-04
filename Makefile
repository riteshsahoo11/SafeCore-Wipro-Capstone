CXX := g++

CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic -Iinclude

TARGET := bin/safecore
TEST_TARGET := bin/test_safety

SOURCES := \
	src/main.cpp \
	src/safety_engine.cpp \
	src/state_manager.cpp \
	src/logger.cpp

OBJECTS := $(SOURCES:.cpp=.o)

TEST_SOURCES := \
	tests/test_safety.cpp \
	src/safety_engine.cpp \
	src/state_manager.cpp

all: user test

user: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin logs
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_SOURCES)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(TEST_SOURCES) -o $(TEST_TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_TARGET)

.PHONY: all user test clean
