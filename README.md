NVME WRITE-THROUGH CACHING ACCELERATOR SIMULATOR

A Linux-based C++17 system programming project that models and evaluates write-through caching strategies for NVMe-style storage.

The project compares a baseline synchronous write-through path with an optimized path using write coalescing, batching, and
multiple NVMe queue pairs.


PROJECT OVERVIEW

The project includes a small Linux kernel character-device module that demonstrates user-space/kernel-space communication.


OBJECTIVES

 * Model a write-through caching layer for NVMe-style storage.
 * Compare synchronous and optimized write paths.
 * Demonstrate write coalescing.
 * Demonstrate batch processing.
 * Model multiple NVMe queue pairs.
 * Measure latency, throughput, IOPS, and queue utilization.
 * Demonstrate Linux system programming concepts.
 * Demonstrate C++ concurrency and resource management.
 * Provide a Linux kernel device-interface component.
 * Maintain a reproducible software-development workflow using Git.


KEY FEATURES


WORKLOAD GENERATION

Supports reproducible logical write workloads with:

 * Uniform distribution
 * Zipfian distribution
 * Configurable address-space size
 * Configurable request count
 * Fixed random seed


BASELINE WRITE-THROUGH CACHE

The baseline implementation:

 * Updates the in-memory cache.
 * Submits one physical write.
 * Uses a single queue.
 * Waits synchronously for completion.

It provides the reference implementation for performance comparison.


OPTIMIZED WRITE-THROUGH CACHE

Supports:

 * Write coalescing
 * Batch processing
 * Background flushing
 * Multiple NVMe queue pairs
 * Concurrent request processing
 * Completion signaling using C++ futures/promises


NVME DEVICE MODEL

The simulator models:

Feature Description Submission queues NVMe-style submission queues Queue workers Workers handling queues Multiple queue pairs
Supports multiple queue pairs Queue depth Depth of each queue Device service latency Latency modeling Physical operation counts
Counts of physical operations Bytes written Data volume Queue utilization Queue utilization metrics

The implementation uses Linux file operations and aligned buffers to model the storage path.


PERFORMANCE METRICS

The project measures:

 * Average latency
 * P50 latency
 * P95 latency
 * P99 latency
 * Maximum latency
 * Physical throughput
 * Application throughput
 * Physical IOPS
 * Application IOPS
 * Storage operation count
 * Coalescing reduction
 * Average queue utilization
 * Per-queue utilization

Detailed results are stored in:

results/comparison.csv

Current benchmark findings indicate that, for the recorded Zipfian workload, the optimized implementation reduced physical storage
operations by approximately 14.31%.

However, it did not produce lower latency or higher throughput than the baseline. This is considered an engineering trade-off
rather than a universal speedup.

Detailed results are available in:

 * docs/baseline-performance.md
 * docs/performance-analysis.md


LINUX KERNEL DEVICE INTERFACE

The project contains a small Linux kernel character-device module located in the driver/ directory.

The module creates the device:

/dev/nvme_wt_sim


It demonstrates:

 * Linux kernel module development
 * Character-device registration
 * User-space/kernel-space communication
 * copy_to_user()
 * copy_from_user()
 * Synchronization
 * Device read/write operations

The module has been tested through stages including module loading, unloading, and read/write operations.

> Important: The kernel module is purely demonstrative. It is not a production NVMe controller driver and does not implement the
> complete NVMe protocol. The main NVMe logic remains in the user-space simulator.


PROJECT STRUCTURE

nvme_wt_sim/
├── include/
│   ├── metrics.hpp
│   ├── nvme_device.hpp
│   ├── workload.hpp
│   └── write_through_cache.hpp
│
├── src/
│   ├── main.cpp
│   ├── metrics.cpp
│   ├── nvme_device.cpp
│   ├── workload.cpp
│   └── write_through_cache.cpp
│
├── tests/
│   ├── test_workload.cpp
│   ├── test_nvme_device.cpp
│   ├── test_write_through_cache.cpp
│   └── test_edge_cases.cpp
│
├── driver/
│   ├── Makefile
│   ├── README.md
│   └── nvme_wt_driver.c
│
├── docs/
│   ├── diagrams/
│   ├── scripts/
│   └── ...
│
├── results/
│
├── Makefile
├── README.md
└── .gitignore



REQUIREMENTS

The project requires:

 * Linux environment
 * C++17 compiler
 * GNU Make
 * Git
 * POSIX development environment


DEVELOPMENT ENVIRONMENT

The primary development environment is:

Windows
   ↓
WSL2
   ↓
Ubuntu Linux


Kernel-module development uses a WSL2 kernel environment with the required kernel build interface.


BUILD THE SIMULATOR

From the project root:

make clean
make


Run the simulator:

make run


Run the test suite:

make test


The test suite includes:

 * Workload tests
 * NVMe device tests
 * Write-through cache tests
 * Edge-case tests


BUILD THE KERNEL MODULE

From the project root:

make -C driver


The generated kernel module is:

driver/nvme_wt_driver.ko


Kernel-module build artifacts are intentionally excluded from Git.


TEST THE KERNEL MODULE

Load the module:

sudo insmod driver/nvme_wt_driver.ko


Check that the device exists:

ls -l /dev/nvme_wt_sim


Write data to the device:

printf "NVMe write-through driver test" | sudo tee /dev/nvme_wt_sim > /dev/null


Read the data:

cat /dev/nvme_wt_sim


Expected output:

NVMe write-through driver test


Unload the module:

sudo rmmod nvme_wt_driver


The /dev/nvme_wt_sim device node should then disappear.

Detailed kernel-module validation is documented in:

docs/driver-validation.md


TESTING AND VALIDATION

The project uses multiple levels of validation:

 * Unit testing
 * Integration testing
 * Edge-case testing
 * System testing
 * Performance testing
 * Kernel module validation

Automated test suites are used to verify the major project components.

Detailed testing documentation is available in:

docs/testing.md

Important project documents include:

 * project-overview.md — project scope and objectives
 * requirements.md — functional and non-functional requirements
 * development-plan.md — implementation and development plan
 * architecture.md — system architecture
 * design.md — detailed class and execution design
 * testing.md — testing strategy
 * test-results.md — test evidence
 * baseline-performance.md — benchmark configuration and baseline
 * performance-analysis.md — performance interpretation
 * driver-validation.md — kernel module validation
 * final-validation.md — final project validation


DEVELOPMENT WORKFLOW

The project follows a structured software-development workflow:

Requirements
     ↓
Architecture
     ↓
Implementation
     ↓
Testing
     ↓
Performance Analysis
     ↓
Improvement
     ↓
Final Validation


Git commits document the progression of the implementation and validation work.


LIMITATIONS

The current project has several limitations:

 * The NVMe implementation is a user-space simulator.
 * It does not implement the complete NVMe protocol.
 * Storage is file-backed.
 * Device latency can be simulated.
 * Benchmark results depend on the host environment.
 * The kernel module is a demonstration character device rather than a production NVMe block driver.


FUTURE WORK

Potential future extensions include:

 * More realistic NVMe command modeling
 * Additional workload distributions
 * Adaptive queue scheduling
 * Advanced write merging
 * More extensive profiling
 * Automated regression benchmarking
 * Hardware-based NVMe validation
 * Expanded Linux device-driver integration


ACADEMIC / TRAINING CONTEXT

This project demonstrates concepts from:

Topic Description Linux Operating system fundamentals C++ Object-oriented and system programming System Programming Low-level
programming and system interfaces Computer Architecture CPU, memory, storage, and I/O concepts Hardware and Software Interaction
Interaction between software and system resources Multithreading Concurrent execution Synchronization Coordination between
concurrent operations Storage Systems Data storage and I/O concepts Performance Analysis System performance evaluation Software
Testing Quality assurance and validation Version Control Git-based software development


AUTHOR

Priyanshu Aman

B.Tech — Computer Science and Engineering
