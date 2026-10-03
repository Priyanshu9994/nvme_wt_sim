#include "workload.hpp"
#include "nvme_device.hpp"
#include "write_through_cache.hpp"

#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

using namespace nvmesim;

namespace {

constexpr size_t BLOCK_SIZE = 4096;
const char* TEST_FILE = "tests/test_edge_storage.bin";

void cleanup() {
    std::remove(TEST_FILE);
}

void test_empty_workload() {
    WorkloadGenerator generator(
        100,
        0,
        Distribution::Uniform,
        1.2,
        42
    );

    auto workload = generator.generate();

    assert(workload.empty());

    std::cout << "[PASS] Empty workload handling\n";
}

void test_single_address_workload() {
    WorkloadGenerator generator(
        1,
        10,
        Distribution::Uniform,
        1.2,
        42
    );

    auto workload = generator.generate();

    assert(workload.size() == 10);

    for (const auto& request : workload) {
        assert(request.lba == 0);
        assert(request.num_blocks > 0);
    }

    std::cout << "[PASS] Single-address workload\n";
}

void test_aligned_buffer() {
    auto buffer = std::make_shared<AlignedBuffer>(BLOCK_SIZE);

    const auto address =
        reinterpret_cast<std::uintptr_t>(buffer->data());

    assert(address % 4096 == 0);
    assert(buffer->size() == BLOCK_SIZE);

    std::memset(buffer->data(), 'X', BLOCK_SIZE);

    assert(buffer->data()[0] == 'X');
    assert(buffer->data()[BLOCK_SIZE - 1] == 'X');

    std::cout << "[PASS] Aligned buffer handling\n";
}

void test_single_queue_device() {
    cleanup();

    NVMeDevice device(
        TEST_FILE,
        8,
        BLOCK_SIZE,
        1,
        0
    );

    auto buffer = std::make_shared<AlignedBuffer>(BLOCK_SIZE);
    std::memset(buffer->data(), 'Y', BLOCK_SIZE);

    auto future = device.submit_write(
        7,
        1,
        buffer,
        0
    );

    future.get();

    assert(device.total_physical_ops() == 1);
    assert(device.total_bytes_written() == BLOCK_SIZE);

    std::cout << "[PASS] Single-queue device boundary\n";
}

void test_repeated_cache_start() {
    cleanup();

    NVMeDevice device(
        TEST_FILE,
        8,
        BLOCK_SIZE,
        2,
        0
    );

    MetricsCollector metrics;

    OptimizedWriteThroughCache cache(
        device,
        metrics,
        std::chrono::milliseconds(20),
        8
    );

    cache.start();
    cache.start();

    std::vector<char> value(BLOCK_SIZE, 'R');
    cache.write(3, value);

    cache.stop();

    assert(device.total_physical_ops() == 1);
    assert(device.total_bytes_written() == BLOCK_SIZE);

    std::cout << "[PASS] Repeated cache start handling\n";
}

void test_immediate_optimized_flush() {
    cleanup();

    NVMeDevice device(
        TEST_FILE,
        8,
        BLOCK_SIZE,
        2,
        0
    );

    MetricsCollector metrics;

    OptimizedWriteThroughCache cache(
        device,
        metrics,
        std::chrono::milliseconds(100),
        1
    );

    cache.start();

    std::vector<char> value(BLOCK_SIZE, 'Z');

    cache.write(0, value);

    cache.stop();

    assert(device.total_physical_ops() == 1);
    assert(device.total_bytes_written() == BLOCK_SIZE);

    std::cout << "[PASS] Immediate batch-trigger handling\n";
}

} // namespace

int main() {
    test_empty_workload();
    test_single_address_workload();
    test_aligned_buffer();
    test_single_queue_device();
    test_immediate_optimized_flush();
    test_repeated_cache_start();

    cleanup();

    std::cout << "\nAll edge-case tests passed.\n";

    return 0;
}
