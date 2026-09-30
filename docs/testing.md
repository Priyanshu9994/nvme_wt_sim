# Testing Strategy

## 1. Testing Objective

The testing strategy verifies the correctness, reliability and performance of
the NVMe Write-Through Caching Accelerator Simulator.

Testing is performed at multiple levels:

- Unit testing
- Integration testing
- System testing
- Performance testing
- Regression testing

---

## 2. Unit Testing

Unit tests verify individual components independently.

### Components

The following components will be tested:

- WorkloadGenerator
- NVMeQueuePair
- NVMeDevice
- BaselineWriteThroughCache
- OptimizedWriteThroughCache
- MetricsCollector

### WorkloadGenerator Tests

Test cases include:

- Valid uniform workload generation
- Valid Zipfian workload generation
- Correct number of generated requests
- Valid LBA range
- Valid block count
- Reproducibility with the same random seed

### NVMe Device Tests

Test cases include:

- Device initialization
- Correct queue count
- Valid write submission
- Physical operation counting
- Byte counting
- Queue statistics
- Invalid write handling

### Cache Tests

Test cases include:

- Basic write operation
- Multiple writes
- Write completion
- Write coalescing
- Batch triggering
- Cache shutdown
- Pending request completion

---

## 3. Integration Testing

Integration tests verify communication between major components.

Examples:

```text
WorkloadGenerator
        |
        v
WriteThroughCache
        |
        v
NVMeDevice
        |
        v
NVMeQueuePair
        |
        v
Storage Backend

# 4. System Testing

System testing evaluates the complete simulator using realistic workloads.

The following configurations will be tested:
- Small workload
- Medium workload
- Large workload
- Uniform distribution
- Zipfian distribution
- Different queue counts
- Different batch sizes
- Different flush intervals

The complete system should execute without crashes, deadlocks, or incomplete write operations.

# 5. Performance Testing

Performance testing measures:
- Average latency
- p50 latency
- p95 latency
- p99 latency
- Maximum latency
- Application throughput
- Physical throughput
- Application IOPS (Input/Output Operations Per Second)
- Physical device IOPS
- Physical operation reduction
- Queue utilization

The baseline implementation will be used as the reference for comparison.

# 6. Regression Testing

'testing ensures that changes to the implementation do not break previously working functionality.

after significant changes, the following commands should succeed:
1. `make clean`
2. `make`
3. `make run`

the generated benchmark results should remain valid and the program should terminate successfully.

# 7. Reproducibility 
Benchmark tests should use fixed configuration values and random seeds when comparing implementations.
Important parameters include:
| Parameter | Description |
| --- | --- |
| Number of requests | Total number of requests to simulate |
| Address-space size | Size of the address space |
| Queue count | Number of queues |
| Workload distribution | Distribution pattern of workload |
| Zipf skew | Skewness factor for Zipfian distribution |
| Flush interval | Interval between flushes |
| Batch size | Number of requests per batch |
| Application thread count | Number of application threads |
| Simulation latency | Latency introduced by simulation |
| Random seed | Seed for random number generator |
This allows performance changes to be compared consistently.

# 8. Test Evidence 
Testing evidence will be maintained through:
test source files, test execution output, benchmark CSV files, git commits, documentation, performance comparisons.
 
definition of test completion 
is considered complete when:
cored modules have unit tests,
manor component interactions have integration tests,
effective workloads execute successfully,
pperformance benchmarks can be reproduced,
dmajor defects are fixed or documented,
test results are committed to the repository,
documentation reflects the final implementation.
