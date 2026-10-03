CXX := g++

CXXFLAGS := -std=c++17 -Wall -Wextra -Werror -pedantic -Iinclude

TARGET := bin/safecore

SOURCES := \
	src/main.cpp \
	src/safety_engine.cpp

OBJECTS := $(SOURCES:.cpp=.o)

all: user

user: $(TARGET)

$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: all user clean
