# Baseline Performance

## 1. Purpose

This document records the initial performance measurements of the NVMe Write-Through Caching Accelerator Simulator.

The measurements establish a baseline for future development and optimization.

All measurements in this document were obtained by running the project under Ubuntu Linux through WSL2.

---

## 2. Test Environment

| Parameter | Value |
|---|---|
| Host OS | Windows |
| Linux Environment | WSL2 |
| Linux Distribution | Ubuntu |
| Programming Language | C++17 |
| Compiler | GNU g++ |
| Build System | GNU Make |
| Workload Requests | 20,000 |
| Address Space | 50,000 |
| Queue Pairs | 8 |
| Workload Distribution | Zipf |
| Zipf Skew | 1.2 |
| Flush Interval | 200 µs |
| Batch Trigger | 64 |
| Application Threads | 8 |
| Random Seed | 42 |
| Simulated Device Latency | 60 µs |

---

## 3. Baseline Configuration

The baseline implementation represents a naive synchronous write-through design.

Characteristics:

- One queue
- Queue depth of one
- Each logical write results in a physical storage operation
- No write coalescing
- No batching
- No multi-queue parallelism

### Baseline Results

| Metric | Result |
|---|---:|
| Logical write requests | 20,000 |
| Physical storage operations | 20,000 |
| Coalescing reduction | 0.00% |
| Wall-clock time | 2.12 s |
| Average latency | 790.03 µs |
| p50 latency | 699.62 µs |
| p95 latency | 1252.83 µs |
| p99 latency | 2086.69 µs |
| Maximum latency | 4976.42 µs |
| Physical throughput | 36.83 MB/s |
| Application throughput | 36.83 MB/s |
| Application IOPS | 9427.81 |
| Physical device IOPS | 9427.81 |
| Average queue utilization | 7.07% |

---

## 4. Optimized Prototype Configuration

The current optimized prototype combines several techniques:

- Write coalescing
- Batched writes
- Multiple queue pairs
- Concurrent request processing

### Optimized Results

| Metric | Result |
|---|---:|
| Logical write requests | 20,000 |
| Physical storage operations | 17,134 |
| Coalescing reduction | 14.33% |
| Wall-clock time | 2.50 s |
| Average latency | 998.57 µs |
| p50 latency | 877.14 µs |
| p95 latency | 1825.30 µs |
| p99 latency | 3048.37 µs |
| Maximum latency | 21550.67 µs |
| Physical throughput | 26.74 MB/s |
| Application throughput | 31.21 MB/s |
| Application IOPS | 7989.01 |
| Physical device IOPS | 6844.19 |
| Average queue utilization | 5.13% |

---

## 5. Comparison

| Metric | Baseline | Optimized | Observed Change |
|---|---:|---:|---|
| Physical operations | 20,000 | 17,134 | 14.33% reduction |
| Average latency | 790.03 µs | 998.57 µs | Increased |
| p99 latency | 2086.69 µs | 3048.37 µs | Increased |
| Physical throughput | 36.83 MB/s | 26.74 MB/s | Decreased |
| Application throughput | 36.83 MB/s | 31.21 MB/s | Decreased |
| Application IOPS | 9427.81 | 7989.01 | Decreased |
| Average queue utilization | 7.07% | 5.13% | Decreased |

---

## 6. Initial Observations

The initial experiment demonstrates that write coalescing successfully reduces the number of physical storage operations.

The optimized configuration reduced physical operations from:

```text
20,000 → 17,134
# Performance Evaluation Report

## Reduction in Physical Storage Operations

This represents a:

- **14.33% reduction** in physical storage operations.

However, the current optimized configuration did not improve latency or throughput for this particular workload. The observed metrics are:

- **Average latency:** increased from 790.03 µs to 998.57 µs
- **Physical throughput:** decreased from 36.83 MB/s to 26.74 MB/s

### Key Insights
The results indicate that reducing physical operations does not automatically translate into better end-to-end performance.

### Factors for Further Investigation
Potential factors to explore include:
- Batch formation overhead
- Queue scheduling overhead
- Thread synchronization
- Coalescing overhead
- Workload characteristics
- Simulated device latency
- Queue utilization
- Batch size
- Flush interval
- Interaction between batching and multi-queue processing

These observations will guide subsequent implementation and testing.

## Reproducibility Details
The benchmark uses a fixed random seed:
```plaintext
eed = 42```
This ensures that the same workload configuration can be reproduced during development.
The current benchmark can be executed using:
```bash
def make run```
The simulator generates the comparison results at:
[results/comparison.csv](results/comparison.csv)

## Baseline Status and Future Directions
This experiment establishes the initial performance baseline of the project.
Future experiments will compare against this baseline to evaluate changes in:
| Aspect | Description |
|---|---|
| Cache behavior | Changes in cache efficiency |
| Coalescing | Data coalescing strategies |
| Batch processing | Handling of batch operations |
| Queue management | Queue handling techniques |
| Threading | Multithreading improvements |
| Device interaction | Communication with hardware devices |
| Driver/interface components | Software interface updates |
 
Performance improvements will be measured based on actual data rather than assumptions.
