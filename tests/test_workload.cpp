#include "workload.hpp"

#include <cassert>
#include <iostream>
#include <set>

using namespace nvmesim;

void test_uniform_workload() {
    const uint64_t address_space = 1000;
    const uint64_t requests = 1000;

    WorkloadGenerator generator(
        address_space,
        requests,
        Distribution::Uniform,
        1.2,
        42
    );

    auto workload = generator.generate();

    assert(workload.size() == requests);

    for (const auto& request : workload) {
        assert(request.lba < address_space);
        assert(request.num_blocks > 0);
    }

    std::cout << "[PASS] Uniform workload generation\n";
}

void test_zipf_workload() {
    const uint64_t address_space = 1000;
    const uint64_t requests = 1000;

    WorkloadGenerator generator(
        address_space,
        requests,
        Distribution::Zipfian,
        1.2,
        42
    );

    auto workload = generator.generate();

    assert(workload.size() == requests);

    for (const auto& request : workload) {
        assert(request.lba < address_space);
        assert(request.num_blocks > 0);
    }

    std::cout << "[PASS] Zipfian workload generation\n";
}

void test_reproducibility() {
    WorkloadGenerator generator1(
        1000,
        100,
        Distribution::Uniform,
        1.2,
        42
    );

    WorkloadGenerator generator2(
        1000,
        100,
        Distribution::Uniform,
        1.2,
        42
    );

    auto workload1 = generator1.generate();
    auto workload2 = generator2.generate();

    assert(workload1.size() == workload2.size());

    for (size_t i = 0; i < workload1.size(); ++i) {
        assert(workload1[i].lba == workload2[i].lba);
        assert(workload1[i].num_blocks == workload2[i].num_blocks);
    }

    std::cout << "[PASS] Workload reproducibility\n";
}

int main() {
    test_uniform_workload();
    test_zipf_workload();
    test_reproducibility();

    std::cout << "\nAll workload tests passed.\n";

    return 0;
}
