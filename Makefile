CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra -Iinclude -pthread

TARGET = nvme_wt_sim

SOURCES = \
	src/main.cpp \
	src/metrics.cpp \
	src/nvme_device.cpp \
	src/workload.cpp \
	src/write_through_cache.cpp

OBJECTS = $(SOURCES:.cpp=.o)

TEST_TARGET = tests/test_workload

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): tests/test_workload.cpp src/workload.cpp
	$(CXX) $(CXXFLAGS) tests/test_workload.cpp src/workload.cpp -o $(TEST_TARGET)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_TARGET)
