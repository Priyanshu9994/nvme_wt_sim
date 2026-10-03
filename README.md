# NVMe Write-Through Caching Accelerator Simulator

A Linux-based C++17 system programming project that models and evaluates
write-through caching strategies for NVMe-style storage.

The project compares a baseline synchronous write-through path with an
optimized path using write coalescing, batching and multiple NVMe queue pairs.

# Project Overview

The project includes a small Linux kernel character-device module that demonstrates user-space/kernel-space communication.

## Objectives
- Model a write-through caching layer for NVMe-style storage.
- Compare synchronous and optimized write paths.
- Demonstrate write coalescing.
- Demonstrate batch processing.
- Model multiple NVMe queue pairs.
- Measure latency, throughput, IOPS, and queue utilization.
- Demonstrate Linux system programming concepts.
- Demonstrate C++ concurrency and resource management.
- Provide a Linux kernel device-interface component.
- Maintain a reproducible software-development workflow using Git.

## Key Features
### Workload Generation
Supports reproducible logical write workloads with:
- Uniform distribution
- Zipfian distribution
- Configurable address-space size
- Configurable request count
- Fixed random seed

### Baseline Write-Through Cache
The baseline implementation:
- Updates the in-memory cache.
- Submits one physical write.
- Uses a single queue.
- Waits synchronously for completion.
It provides the reference implementation for performance comparison.

### Optimized Write-Through Cache
Supports:
- Write coalescing
- Batch processing
- Background flushing
- Multiple NVMe queue pairs
- Concurrent request processing
- Completion signaling using C++ futures/promises

### NVMe Device Model
Models:
| Feature | Description |
| --- | --- |
| Submission queues | NVMe-style submission queues |
| Queue workers | Workers handling queues |
| Multiple queue pairs | Supports multiple pairs |
| Queue depth | Depth of each queue |
| Device service latency | Latency modeling |
| Physical operation counts | Counts of operations |
| Bytes written | Data volume |
| Queue utilization | Utilization metrics |
The implementation uses Linux file operations and aligned buffers to model storage path.

## Performance Metrics 
Measures include:
average latency, p50/p95/p99 latency, maximum latency, physical throughput, application throughput, IOPS (physical and application), storage operations, coalescing reduction, queue utilization (average and per queue).
details are stored in `results/comparison.csv`.
Current benchmark findings indicate that for the recorded Zipfian workload, the optimized implementation reduced physical storage operations by approximately **14.31%**. However, it did not produce lower latency or higher throughput than the baseline. This is considered an engineering trade-off rather than a universal speedup. 
detailed results are available in:
docs/baseline-performance.md and docs/performance-analysis.md.

# Linux Kernel Device Interface 
The project contains a small Linux kernel character-device module:
driver/
b├── Makefile  
b├── README.md  
b└── nvme_wt_driver.c  
named `/dev/nvme_wt_sim` which demonstrates:
lkernel module development,
e.g., character-device registration,
copied to/from user space functions (`copy_to_user()`, `copy_from_user()`), synchronization,
and user-space/kernel-space communication. The module has been tested through various stages including load/unload and read/write operations. It is important to note that this kernel module is purely demonstrative; it is not a production NVMe controller driver nor a complete protocol implementation. The main NVMe logic remains in the userspace simulator.

default structure of project files includes directories like `nvme_wt_sim/`, `include/`, `src/`, `tests/`, `driver/`, `docs/`, etc., each containing relevant source code, tests, documentation, diagrams, scripts, results, build files (`Makefile`), README files, and `.gitignore` for version control management.
'the environment requirements include Linux (preferably Ubuntu via WSL2 on Windows), C++17 compiler, GNU Make, Git, POSIX environment. Build commands include `make clean && make` from root to compile the simulator; run with `make run`. Tests can be executed via `make test`. Kernel module can be built with `make -C driver` and loaded/unloaded using standard insmod/rmmod commands with validation steps outlined above.'} } }}}}
# Important

The kernel module is a demonstration character-device interface.

It is not:

- A production NVMe controller driver
- A replacement for the Linux NVMe subsystem
- A complete NVMe protocol implementation
- A production block-storage driver

The main NVMe implementation remains a userspace simulator.

## Project Structure
```
nvme_wt_sim/
├── include/
│   ├── metrics.hpp
│   ├── nvme_device.hpp
│   ├── workload.hpp
│   └── write_through_cache.hpp
├── src/
│   ├── main.cpp
│   ├── metrics.cpp
│   ├── nvme_device.cpp
│   ├── workload.cpp
│   └── write_through_cache.cpp
├── tests/
│   ├── test_workload.cpp
│   ├── test_nvme_device.cpp
│   ├── test_write_through_cache.cpp
│   └── test_edge_cases.cpp
├── driver/
│   ├── Makefile
docs/
├── diagrams/
├── scripts/
├── results/
├── Makefile
├── README.md
└── .gitignore``` 
 
## Requirements 
* Linux environment 
* C++17 compiler 
* GNU Make 
* Git 
* POSIX development environment 
 
The primary development environment is:
> Windows → WSL2 → Ubuntu Linux  
Kernel-module development uses a custom Microsoft WSL2 kernel with the required kernel build interface.
 
## Build the Simulator 
From the project root:
```bash
targets: make clean, make, make run```
Run the Simulator:
default command:
make run  
the benchmark comparison is written to:
docs/compare.csv  
test suite: make test  
the suite includes: workload tests, NVMe device tests, write-through cache tests, edge-case tests.
 
build the Kernel Module: from project root:
makes -C driver  
the generated module is: driver/nvme_wt_driver.ko  
the kernel-module build artifacts are intentionally excluded from Git.
Test the Kernel Module:
sudo insmod driver/nvme_wt_driver.ko  check: ls -l /dev/nvme_wt_sim  write: printf "NVMe write-through driver test" | sudo tee /dev/nvme_wt_sim > /dev/null  read: cat /dev/nvme_wt_sim  expected output: NVMe write-through driver test  unload: sudo rmmod nvme_wt_driver  The device node should then disappear.
detailed validation is documented in docs/driver-validation.md.
 
## Testing and Validation 
the project uses multiple levels of validation including unit testing, integration testing, edge-case testing, system testing, performance testing, and kernel module validation. All automated test suites passed during recorded validation runs.
 
documentation: detailed project documentation is available in docs/. important documents include project-overview.md (project scope and objectives), requirements.md (functional and non-functional requirements), development-plan.md (implementation and development plan), architecture.md (system architecture), design.md (detailed class and execution design), testing.md (testing strategy), test-results.md (test evidence), baseline-performance.md (benchmark configuration and baseline), performance-analysis.md (performance interpretation), driver-validation.md (kernel module validation), final-validation.md (final project validation).
 
building workflow follows these steps:
git commits document progression of implementation and validation work.
e.g., requirements → architecture → implementation → testing → performance analysis → improvement → final validation.
'the current limitations include that the NVMe implementation is a userspace simulator; it does not implement full protocol; storage is file-backed; device latency can be simulated; benchmark results depend on host environment; kernel module is demonstration only.
future work includes more realistic command modeling, additional workloads, adaptive scheduling, advanced merging, profiling, regression benchmarking, hardware validation, Linux integration.
demonstrates concepts from Linux system programming, computer architecture, hardware/software interaction,
multithreading,
synchronization,
storage systems,
pPerformance analysis,
sftware testing,
and version control.
host author info: Priyanshu Aman — B.Tech in Computer Science and Engineering.
# Documentation

Detailed project documentation is available in `docs/`.

## Important Documents
- `project-overview.md` — project scope and objectives
- `requirements.md` — functional and non-functional requirements
- `development-plan.md` — implementation and development plan
- `architecture.md` — system architecture
- `design.md` — detailed class and execution design
- `testing.md` — testing strategy
- `test-results.md` — test evidence
- `baseline-performance.md` — benchmark configuration and baseline
- `performance-analysis.md` — performance interpretation
- `driver-validation.md` — kernel module validation
- `final-validation.md` — final project validation

## Development Workflow
The project follows a professional development workflow:
```
Requirements → Architecture → Implementation → Testing → Performance Analysis → Improvement → Final Validation
```
Git commits document the progression of the implementation and validation work.

## Limitations
The current project has several limitations:
- The NVMe implementation is a userspace simulator.
- It does not implement the complete NVMe protocol.
- Storage is file-backed.
- Device latency can be simulated.
- Benchmark results depend on the host environment.
- The kernel module is a demonstration character device rather than a production NVMe block driver.

## Future Work
Potential extensions include:
- More realistic NVMe command modeling
- Additional workload distributions
 - Adaptive queue scheduling
 - Advanced write merging
 - More extensive profiling
 - Automated regression benchmarking
 - Hardware-based NVMe validation
 - Expanded Linux device-driver integration

## Academic / Training Context
This project demonstrates concepts from:
| Topic | Description |
|---------|--------------|
| Linux | Operating system fundamentals |
| C++ | Programming language |
| System Programming | Low-level programming |
| Computer Architecture | Hardware design principles |
| Hardware and Software Interaction | Integration concepts |
| Multithreading | Concurrent execution |
| Synchronization | Coordination mechanisms |
| Storage Systems | Data storage solutions |
| Performance Analysis | System evaluation techniques |
| Software Testing | Quality assurance processes |
| Version Control | Code management tools |

## Author 
**Priyanshu Aman**
B.Tech — Computer Science and Engineering
