# System Design

## 1. Design Overview

The NVMe Write-Through Caching Accelerator Simulator is designed as a layered
Linux-based C++ system.

The system models the path of an application write request from workload
generation through the write-through cache and finally to an NVMe-like storage
device.

The design separates the system into the following major components:

1. Workload Generation
2. Write-Through Cache
3. NVMe Device Model
4. NVMe Queue Pairs
5. Linux File-Based Storage
6. Metrics Collection
7. Benchmark and Result Generation

The design supports two execution paths:

- Baseline synchronous write-through path
- Optimized coalesced, batched and multi-queue write-through path

The current implementation is a userspace simulator. It models NVMe-style
device behavior and Linux storage operations but is not a production Linux
kernel NVMe driver.

---

## 2. High-Level Architecture

```mermaid
flowchart TB

    APP["Application / Benchmark"]

    WG["WorkloadGenerator"]
    WR["WorkloadRequest"]

    BC["BaselineWriteThroughCache"]
    OC["OptimizedWriteThroughCache"]

    CO["Write Coalescing"]
    BA["Batch Processing"]

    DEV["NVMeDevice"]

    Q1["NVMeQueuePair 0"]
    Q2["NVMeQueuePair 1"]
    Q3["NVMeQueuePair ..."]
    QN["NVMeQueuePair N"]

    ST["Linux File-Based Storage"]
    MET["MetricsCollector"]

    APP --> WG
    WG --> WR

    WR --> BC
    WR --> OC
    OC --> CO
    CO --> BA
    BA --> DEV

    BC --> DEV

    DEV --> Q1
    DEV --> Q2
    DEV --> Q3
    DEV --> QN

    Q1 --> ST
    Q2 --> ST
    Q3 --> ST
    QN --> ST

    BC --> MET
    OC --> MET
    DEV --> MET

# 3. Architectural Layers

## 3.1 Application and Workload Layer

The workload layer generates logical write requests for the simulator.

### Main components:
- `WorkloadGenerator`
- `WorkloadRequest`

`WorkloadRequest` represents a logical write operation using:
- Logical block address (LBA)
- Number of blocks

WorkloadGenerator supports different request distributions including:
- Uniform distribution
- Zipfian distribution

The workload generator is independent of the storage implementation.

## 3.2 Cache Layer

The cache layer provides the write-through behavior.

### Available implementations:
- `BaselineWriteThroughCache`
- `OptimizedWriteThroughCache`

Both implementations receive write requests and eventually submit physical write operations to the NVMe device model.

### Baseline Cache
The baseline implementation follows a simple synchronous path:
1. Receive logical write.
2. Update the cache.
3. Create a device write request.
4. Submit the request to the device.
5. Wait for completion.
6. Return to the application.

The baseline uses a single queue and effectively operates with queue depth one.

### Optimized Cache
The optimized implementation adds:
- Write coalescing
- Batch processing
- Background flushing
- Multiple queue pairs
- Concurrent request processing

the optimized implementation maintains pending writes and combines compatible requests before submitting them to the device.

## 4. Device Layer

the device layer is represented by the `NVMeDevice` class, responsible for:
- Managing backing storage file.
- Managing NVMe queue pairs.
- Receiving write submissions.
- Selecting queues.
- Tracking physical operations, bytes written, device activity, and per-queue utilization.

the device owns multiple `NVMeQueuePair` objects, modeled as:
```
nvme_device   nvme_queue_pair 0 
|
+-- nvme_queue_pair 1 
|
+-- nvme_queue_pair 2 
|
+-- ...
|
+-- nvme_queue_pair N```
designed to model parallel storage processing with multiple submission/completion paths.

## 5. Queue Pair Design
Each `NVMeQueuePair` represents an independent simulated device queue containing:
a) Submission queue,
b) Worker thread,c) Queue synchronization primitives,
d) Queue statistics,
e) Simulated device latency.
the queue uses:
a) `std::dequ

e` for pending commands,
b) `std::mutex` for synchronization,
c) `std::condition_variable` for worker notification,
d) `std::thread` for processing,
e) Atomic state variables for running/stopping status.
defining Queue Processing mechanics.

## 6. Core Data Structures & Relationships \
and Operations Overview:
purpose of each structure like WorkloadRequest, AlignedBuffer, WriteCommand, QueueStats, etc., along with their responsibilities and conceptual models are detailed in sections 6.1 through 6.4 and beyond, covering class relationships and system design considerations.


# 20. Repository and Version Control Design

The repository follows a professional software-project structure.

```
nvme_wt_sim/
├── include/
6.2 AlignedBuffer

AlignedBuffer provides RAII management for aligned memory.

The buffer uses aligned allocation suitable for direct I/O.

Responsibilities:

Allocate aligned memory.
Provide access to the buffer.
Release memory automatically.
Prevent accidental copying.

Conceptually:

AlignedBuffer
    |
    +-- memory pointer
    +-- buffer size
6.3 WriteCommand

A WriteCommand represents a physical device write.

WriteCommand
    |
    +-- lba
    +-- num_blocks
    +-- data
    +-- enqueue_time
    +-- completion
    +-- merged_request_count

The command also stores information required to track completion and
coalescing.

merged_request_count allows the system to determine how many logical
requests are represented by a physical command.

6.4 QueueStats

Each queue maintains statistics including:

Operations completed
Bytes written
Busy time
Maximum queue depth observed

These statistics are later used to calculate queue utilization and device
performance.

7. Class Relationships
8. Baseline Write Path

The baseline path is intentionally simple so that it can be used as a
reference for evaluating optimization techniques.

Baseline Characteristics
One active queue.
Synchronous submission.
One physical operation per logical request.
No write coalescing.
No batch-based optimization.
Simple control flow.

The baseline is used as the reference implementation for performance
comparison.

9. Optimized Write Path

The optimized path introduces a pipeline between logical application
writes and physical device operations.

10. Write Coalescing

Write coalescing reduces redundant physical operations when multiple logical
writes target the same logical block during the pending period.

Conceptually:

Logical Requests

Write LBA 100
Write LBA 100
Write LBA 100
Write LBA 200

        |
        v

Coalescing

LBA 100 -> latest data
LBA 200 -> latest data

        |
        v

Physical Writes

Write LBA 100
Write LBA 200

The number of logical requests can therefore be larger than the number of
physical storage operations.

The system records this relationship through physical operation counts and
the merged request information stored in WriteCommand.

11. Batch Processing

The optimized cache supports a configurable batch trigger.

A flush can occur when:

The pending write count reaches the batch threshold.
The configured flush interval expires.
The cache is explicitly stopped or flushed.

Batch processing reduces the overhead of independently submitting every
logical request.

The batch is then dispatched to the NVMe device layer.

12. Multi-Queue Processing

The device model supports multiple queue pairs.

The optimized path can distribute physical operations across available
queues.

Conceptually:

                    NVMeDevice
                        |
        +---------------+---------------+
        |               |               |
        v               v               v
     Queue 0         Queue 1         Queue 2
        |               |               |
        +---------------+---------------+
                        |
                        v
                  Storage Backend

Queue statistics allow the system to measure whether work is being
distributed across the available queues.

13. Concurrency Model

The project uses C++ concurrency primitives to model concurrent storage
processing.

Important concurrency components include:

Application worker threads.
Queue worker threads.
Optimized cache flush thread.
Mutexes.
Condition variables.
Atomic state variables.
Futures and promises for completion signaling.
Thread Model
Application Threads
        |
        v
Optimized Cache
        |
        v
Flush Thread
        |
        v
NVMe Device
        |
   +----+----+----+
   |    |    |    |
   v    v    v    v
 Q0   Q1   Q2   ... QN
 |    |    |       |
 +----+----+-------+
          |
          v
      Storage
14. Optimized Cache State Machine
15. Metrics and Observability

The simulator collects metrics for evaluating both implementations.

Important measurements include:

Logical write requests
Physical storage operations
Coalescing reduction
Wall-clock execution time
Average latency
p50 latency
p95 latency
p99 latency
Maximum latency
Physical throughput
Application throughput
Application IOPS
Physical device IOPS
Average queue utilization
Per-queue utilization

The comparison output is written to CSV so that benchmark results can be
processed and visualized independently.

16. Error Handling Strategy

The system should detect and report failures at the following boundaries:

Input Layer
Invalid configuration values
Invalid workload parameters
Invalid queue count
Invalid address space
Cache Layer
Invalid logical block address
Invalid write size
Failed pending-request handling
Thread shutdown problems
Device Layer
Invalid queue selection
Failed storage operations
File open failures
File allocation failures
Write failures
Concurrency Layer
Worker thread shutdown
Queue synchronization
Completion signaling
Pending request cleanup

Errors should be handled without silently corrupting benchmark results.

17. Linux System Programming Boundary

The project uses Linux system programming concepts in the storage backend.

The device model interacts with a backing storage file using Linux file
operations and direct-I/O related mechanisms.

The design therefore demonstrates:

File descriptors
File allocation
Direct I/O
Aligned memory
Positional writes
Threads
Synchronization primitives
Atomic operations

The current project is a userspace simulator.

A future extension may introduce a dedicated Linux device-driver or kernel
interface if the development environment supports safe kernel-module
development.

The simulator must not be described as a kernel driver unless such a
component is actually implemented and tested.

18. Device Driver Extension Boundary

The architecture intentionally keeps the storage/device layer separated from
the cache and workload layers.

This allows a future driver-backed implementation to replace or extend the
current userspace device model.

Current architecture:

Application
    |
    v
Write-Through Cache
    |
    v
NVMeDevice Simulator
    |
    v
Linux File Storage

Potential future architecture:

Application
    |
    v
Write-Through Cache
    |
    v
Device Interface
    |
    v
Linux Device Driver
    |
    v
Block / NVMe Subsystem
    |
    v
Storage Hardware

The driver extension is considered an architectural boundary rather than a
claim about the current implementation.

19. Deployment and Development Environment

Development environment:

Windows host
WSL2
Ubuntu Linux
GNU C++ compiler
C++17
GNU Make
Git

The project is developed and executed directly inside the WSL2 Linux
environment.

The current build system uses a Makefile.

Primary build commands:

make clean
make
make run
├── src/
├── tests/
├── driver/
├── scripts/
├── results/
├── diagrams/
├── docs/
├── Makefile
├── README.md
└── .gitignore
```

The repository does not expose internal training-stage names in its directory structure.

Development progress is represented through:
- Git commits
- Documentation updates
- Architecture changes
- Test additions
- Benchmark results
- Implementation improvements

**Example commit progression:**
1. Initial working NVMe write-through simulator
2. Add project documentation and baseline performance
3. Add project requirements and development plan
4. Add system architecture documentation
5. Add detailed system design
6. Improve write coalescing
7. Improve batch processing
8. Add device interface
9. Add unit tests
10. Add integration tests
11. Add benchmark analysis
12. Improve error handling
13. Update documentation
14. Prepare final release
---
# 21. Design Decisions
## Decision 1: Userspace Simulator
The project currently uses a userspace simulator rather than immediately implementing a kernel driver.
**Reason:**
- Easier development and debugging.
- Allows performance experiments without requiring real NVMe hardware.
- Provides a controlled environment for testing caching strategies.
## Decision 2: Separate Baseline and Optimized Paths
The baseline implementation is preserved rather than replaced.
**Reason:**
- Provides a reference implementation.
- Makes performance comparison possible.
- Allows optimization effects to be measured objectively.
## Decision 3: Multiple Queue Pairs
The device model supports multiple queue pairs.
**Reason:**
- Represents parallel device processing.
- Allows queue utilization to be measured.
- Provides a foundation for concurrent I/O experiments.
## Decision 4: File-Backed Storage
The simulator uses file-backed storage.
**Reason:**
- Reproducible development environment.
- No dependency on dedicated NVMe hardware.
- Compatible with WSL2 development.
---
# 22. Current Design Limitations
The current architecture has several limitations:
e.g.,
does not implement the complete NVMe protocol, no production Linux kernel driver, file-based backing storage, simulated device latency, environment-dependent benchmark results, trade-offs in latency or throughput based on workload characteristics.
the limitations are part of the current scope and will be considered during testing and future improvements.
'these include incomplete protocol implementation, lack of production driver, storage backend type, latency simulation, environment dependence, and workload trade-offs.'
'they will guide ongoing development.'

# 23. Future Extensions
Potential future improvements include:
Linux device-driver integration,
More realistic NVMe command modeling,
Additional workload distributions,
Dynamic queue scheduling,
Improved cache eviction policies,
More advanced write merging,
Additional storage backends,
Automated regression benchmarks,
Expanded unit and integration testing,
Performance profiling,
Hardware-based NVMe validation where available.'

# 24. Design Summary
The system follows a layered architecture that separates workload generation, caching, device simulation, queue processing, storage and measurement.

the architecture provides a clear path from:
 - Logical Application Write 
 - Write-Through Cache 
 - Coalescing / Batching 
 - NVMe Device Model 
 - Multiple Queue Pairs 
 - Linux Storage
this separation makes it possible to compare the baseline and optimized implementations, measure their behavior, add testing progressively and extend the system toward a Linux device-interface or driver-based design.
