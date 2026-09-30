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

TEST_TARGETS = tests/test_workload tests/test_nvme_device

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

tests/test_workload: tests/test_workload.cpp src/workload.cpp
	$(CXX) $(CXXFLAGS) tests/test_workload.cpp src/workload.cpp -o tests/test_workload

tests/test_nvme_device: tests/test_nvme_device.cpp src/nvme_device.cpp
	$(CXX) $(CXXFLAGS) tests/test_nvme_device.cpp src/nvme_device.cpp -o tests/test_nvme_device

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGETS)
	./tests/test_workload
	./tests/test_nvme_device

clean:
	rm -f $(OBJECTS) $(TARGET) $(TEST_TARGETS)
