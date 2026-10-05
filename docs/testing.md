# Testing and Validation


Now create the performance analysis:

```bash id="perf-doc"
cat > docs/performance-analysis.md <<'EOF'
# Performance Analysis

## 1. Benchmark Configuration

The benchmark compares:

1. Baseline synchronous write-through
2. Optimized coalesced, batched and multi-queue write-through

Configuration:

| Parameter | Value |
|---|---:|
| Logical requests | 20,000 |
| Address space | 50,000 blocks |
| Queues | 8 |
| Distribution | Zipfian |
| Zipf skew | 1.2 |
| Flush interval | 200 us |
| Batch trigger | 64 |
| Application threads | 8 |
| Random seed | 42 |
| Simulated latency | 60 us |

---

## 2. Latest Benchmark Results

### Baseline

| Metric | Result |
|---|---:|
| Physical operations | 20,000 |
| Average latency | 921.48 us |
| p50 latency | 740.95 us |
| p95 latency | 1842.14 us |
| p99 latency | 2977.43 us |
| Maximum latency | 8877.84 us |
| Physical throughput | 31.18 MB/s |
| Application throughput | 31.18 MB/s |
| Application IOPS | 7982.31 |
| Physical device IOPS | 7982.31 |
| Average queue utilization | 5.98% |

---

## 3. Latest Optimized Results

| Metric | Result |
|---|---:|
| Physical operations | 17,139 |
| Coalescing reduction | 14.31% |
| Average latency | 1173.17 us |
| p50 latency | 1000.34 us |
| p95 latency | 2303.34 us |
| p99 latency | 4145.33 us |
| Maximum latency | 10672.82 us |
| Physical throughput | 22.74 MB/s |
| Application throughput | 26.54 MB/s |
| Application IOPS | 6794.26 |
| Physical device IOPS | 5822.34 |
| Average queue utilization | 4.36% |

---

# 4. Observations

The optimized implementation reduced the number of physical storage operations from 20,000 to 17,139.

This corresponds to a physical-operation reduction of:

```
14.31%
```

However, the optimized configuration did not improve the measured latency or throughput for this benchmark.

Compared with the baseline:
- Average latency increased from 921.48 us to 1173.17 us.
- p99 latency increased from 2977.43 us to 4145.33 us.
- Physical throughput decreased from 31.18 MB/s to 22.74 MB/s.
- Application throughput decreased from 31.18 MB/s to 26.54 MB/s.
- Application IOPS decreased from 7982.31 to 6794.26.
- Physical device IOPS decreased from 7982.31 to 5822.34.
- Average queue utilization changed from 5.98% to 4.36%.

The main positive result is the reduction in physical storage operations through write coalescing.

---

# 5. Interpretation

The benchmark demonstrates that reducing physical storage operations does not automatically result in lower latency or higher throughput.

The observed performance can be affected by:
- Flush interval
- Batch size
- Background flush-thread scheduling
- Workload distribution
- Simulated device latency
- File-backed storage behavior
- WSL2 execution overhead
- Host scheduling variability

Therefore, the current optimized implementation should be evaluated as a trade-off between physical-operation reduction and observed application performance.

---

# 6. Engineering Finding

The current implementation successfully demonstrates write coalescing by reducing physical storage operations for the tested Zipfian workload.

At the same time, the benchmark shows that the current configuration does not provide a latency or throughput improvement over the baseline.

t provides a useful basis for further optimization and experimentation.
Future experiments should vary:
- Batch size
- Flush interval
 - Number of queues
 - Workload distribution
 - Zipf skew
 - Simulated device latency
 - Application thread count
 
---
 
# 7. Reproducibility
 
rhe benchmark can be rerun using:
 
bashmake run
 
dhe comparison data is written to:
 
bresults/comparison.csv
 
dhe fixed random seed and benchmark parameters allow repeated runs to be compared consistently.
 
dhe conclusion
 
the Stage 5 performance evaluation confirms that the simulator is able to measure both logical application behavior and physical device behavior.
 
the current benchmark demonstrates:
 	Successful write coalescing
 	Reduction in physical storage operations
 	Multiqueue operation
 	Repeatable performance measurement
 	Quantitative comparison between baseline and optimized paths
 
the results also identify areas where additional tuning is required before the optimized configuration can improve latency or throughput for this workload.


---

# Final Validation

## Project Validation Summary

The NVMe Write-Through Caching Accelerator Simulator was validated as a
complete Linux-based C++ project.

## Software Validation

The following automated test groups were executed:

- Workload generation tests
- NVMe device and queue tests
- Write-through cache integration tests
- Edge-case and reliability tests

All implemented tests passed during final validation.

## Kernel Module Validation

The Linux kernel module was:

1. Compiled against the custom WSL2 kernel.
2. Loaded using `insmod`.
3. Registered as `/dev/nvme_wt_sim`.
4. Tested using user-space write and read operations.
5. Unloaded using `rmmod`.
6. Verified to remove the device node after unloading.

## Performance Validation

The final benchmark compares the baseline synchronous write-through path
with the optimized coalesced, batched and multi-queue path.

The tested configuration uses:

- 20,000 logical requests
- 50,000-block address space
- 8 queue pairs
- Zipfian workload
- Zipf skew 1.2
- 200 microsecond flush interval
- Batch trigger of 64
- 8 application threads
- Fixed random seed of 42
- 60 microsecond simulated latency

The benchmark demonstrates a reduction in physical storage operations for the
optimized path.

The measured benchmark also shows that this configuration does not produce
lower latency or higher throughput than the baseline. These results are
documented as an engineering trade-off rather than being presented as a
universal performance improvement.

## Development Environment

- Windows host
- WSL2
- Ubuntu Linux
- Custom Microsoft WSL kernel
- C++17
- GNU C++
- GNU Make
- Git

## Project Status

The project includes:

- Source implementation
- Automated tests
- Linux kernel device-interface demonstration
- Architecture documentation
- Detailed design
- Requirements
- Development plan
- Benchmark results
- Performance analysis
- Driver validation
- Final validation documentation

## Limitations

The project does not implement a complete production NVMe controller or
production Linux NVMe block driver.

The kernel module is a demonstration character-device interface, while the
main NVMe implementation remains a userspace simulator with file-backed
storage.

## Future Work

Potential future improvements include:

- More realistic NVMe command modeling
- Additional workload models
- Adaptive queue scheduling
- More advanced write merging
- Hardware-based NVMe validation
- Further batch and flush tuning
- Expanded performance profiling
