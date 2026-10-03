#include "write_through_cache.hpp"

#include <atomic>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <thread>
#include <vector>

using namespace nvmesim;

namespace {

constexpr size_t BLOCK_SIZE = 4096;
constexpr uint64_t NUM_BLOCKS = 64;

const char* TEST_FILE_BASELINE = "tests/test_cache_baseline.bin";
const char* TEST_FILE_OPTIMIZED = "tests/test_cache_optimized.bin";

void cleanup_files() {
    std::remove(TEST_FILE_BASELINE);
    std::remove(TEST_FILE_OPTIMIZED);
}

std::vector<char> make_value(char value) {
    return std::vector<char>(BLOCK_SIZE, value);
}

void test_baseline_cache_write() {
    std::remove(TEST_FILE_BASELINE);

    NVMeDevice device(
        TEST_FILE_BASELINE,
        NUM_BLOCKS,
        BLOCK_SIZE,
        2,
        0
    );

    MetricsCollector metrics;
    BaselineWriteThroughCache cache(device, metrics);

    const auto value = make_value('A');

    cache.write(0, value);
    cache.write(1, value);
    cache.write(2, value);

    assert(device.total_physical_ops() == 3);
    assert(device.total_bytes_written() == 3 * BLOCK_SIZE);

    std::cout << "[PASS] Baseline cache writes\n";
}

void test_optimized_cache_write() {
    std::remove(TEST_FILE_OPTIMIZED);

    NVMeDevice device(
        TEST_FILE_OPTIMIZED,
        NUM_BLOCKS,
        BLOCK_SIZE,
        4,
        0
    );

    MetricsCollector metrics;

    OptimizedWriteThroughCache cache(
        device,
        metrics,
        std::chrono::milliseconds(20),
        64
    );

    cache.start();

    const auto value = make_value('B');

    cache.write(0, value);
    cache.write(1, value);
    cache.write(2, value);

    cache.stop();

    assert(device.total_physical_ops() == 3);
    assert(device.total_bytes_written() == 3 * BLOCK_SIZE);

    std::cout << "[PASS] Optimized cache writes\n";
}

void test_optimized_cache_coalescing() {
    std::remove(TEST_FILE_OPTIMIZED);

    NVMeDevice device(
        TEST_FILE_OPTIMIZED,
        NUM_BLOCKS,
        BLOCK_SIZE,
        4,
        0
    );

    MetricsCollector metrics;

    OptimizedWriteThroughCache cache(
        device,
        metrics,
        std::chrono::milliseconds(100),
        64
    );

    cache.start();

    constexpr int NUM_WRITERS = 4;

    std::atomic<bool> start{false};
    std::vector<std::thread> workers;

    for (int i = 0; i < NUM_WRITERS; ++i) {
        workers.emplace_back([&cache, &start, i]() {
            while (!start.load(std::memory_order_acquire)) {
                std::this_thread::yield();
            }

            auto value = make_value(static_cast<char>('C' + i));
            cache.write(10, value);
        });
    }

    start.store(true, std::memory_order_release);

    for (auto& worker : workers) {
        worker.join();
    }

    cache.stop();

    assert(device.total_physical_ops() == 1);
    assert(device.total_bytes_written() == BLOCK_SIZE);

    std::cout << "[PASS] Optimized cache coalescing\n";
}

void test_cache_lifecycle() {
    std::remove(TEST_FILE_OPTIMIZED);

    NVMeDevice device(
        TEST_FILE_OPTIMIZED,
        NUM_BLOCKS,
        BLOCK_SIZE,
        2,
        0
    );

    MetricsCollector metrics;

    OptimizedWriteThroughCache cache(
        device,
        metrics,
        std::chrono::milliseconds(10),
        8
    );

    cache.start();
    cache.stop();

    cache.start();

    const auto value = make_value('D');
    cache.write(4, value);

    cache.stop();

    assert(device.total_physical_ops() == 1);

    std::cout << "[PASS] Optimized cache lifecycle\n";
}

} // namespace

int main() {
    test_baseline_cache_write();
    test_optimized_cache_write();
    test_optimized_cache_coalescing();
    test_cache_lifecycle();

    cleanup_files();

    std::cout << "\nAll write-through cache tests passed.\n";

    return 0;
}
