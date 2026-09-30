#include "nvme_device.hpp"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>

using namespace nvmesim;

namespace {

constexpr size_t BLOCK_SIZE = 4096;
constexpr uint64_t NUM_BLOCKS = 64;

const char* TEST_FILE = "tests/test_nvme_storage.bin";

void cleanup_test_file() {
    std::remove(TEST_FILE);
}

void test_device_initialization() {
    cleanup_test_file();

    NVMeDevice device(
        TEST_FILE,
        NUM_BLOCKS,
        BLOCK_SIZE,
        4,
        0
    );

    assert(device.num_queues() == 4);
    assert(device.block_size() == BLOCK_SIZE);

    std::cout << "[PASS] NVMe device initialization\n";
}

void test_single_write() {
    cleanup_test_file();

    NVMeDevice device(
        TEST_FILE,
        NUM_BLOCKS,
        BLOCK_SIZE,
        2,
        0
    );

    auto buffer = std::make_shared<AlignedBuffer>(BLOCK_SIZE);

    std::memset(buffer->data(), 'A', BLOCK_SIZE);

    auto completion = device.submit_write(
        0,
        1,
        buffer,
        0
    );

    completion.get();

    assert(device.total_physical_ops() == 1);
    assert(device.total_bytes_written() == BLOCK_SIZE);

    std::cout << "[PASS] Single NVMe write\n";
}

void test_multiple_writes() {
    cleanup_test_file();

    NVMeDevice device(
        TEST_FILE,
        NUM_BLOCKS,
        BLOCK_SIZE,
        4,
        0
    );

    for (uint64_t i = 0; i < 8; ++i) {
        auto buffer = std::make_shared<AlignedBuffer>(BLOCK_SIZE);

        std::memset(
            buffer->data(),
            static_cast<int>('A' + (i % 26)),
            BLOCK_SIZE
        );

        auto completion = device.submit_write(
            i,
            1,
            buffer,
            static_cast<int>(i % 4)
        );

        completion.get();
    }

    assert(device.total_physical_ops() == 8);
    assert(device.total_bytes_written() == 8 * BLOCK_SIZE);

    std::cout << "[PASS] Multiple NVMe writes\n";
}

void test_queue_selection() {
    cleanup_test_file();

    NVMeDevice device(
        TEST_FILE,
        NUM_BLOCKS,
        BLOCK_SIZE,
        4,
        0
    );

    for (uint64_t i = 0; i < 4; ++i) {
        auto buffer = std::make_shared<AlignedBuffer>(BLOCK_SIZE);

        std::memset(buffer->data(), 'Q', BLOCK_SIZE);

        auto completion = device.submit_write(
            i,
            1,
            buffer,
            static_cast<int>(i)
        );

        completion.get();
    }

    assert(device.total_physical_ops() == 4);

    std::cout << "[PASS] Multiple queue selection\n";
}

} // namespace

int main() {
    test_device_initialization();
    test_single_write();
    test_multiple_writes();
    test_queue_selection();

    cleanup_test_file();

    std::cout << "\nAll NVMe device tests passed.\n";

    return 0;
}
