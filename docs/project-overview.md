# NVMe Write-Through Caching Accelerator Simulator

## 1. Project Overview

The NVMe Write-Through Caching Accelerator Simulator is a Linux-based C++ system programming project designed to study and evaluate techniques for improving write performance in storage systems.

The project models a write-through caching layer positioned between an application and an NVMe-based storage device. It provides a baseline synchronous write path and an optimized write path using write coalescing, batching, and multiple NVMe queue pairs.

The simulator measures important storage-system metrics such as latency, throughput, IOPS, physical storage operations, and queue utilization.

The project is being developed and tested using Ubuntu running through WSL2 on Windows.

---

## 2. Problem Statement

Modern storage systems receive a large number of write requests from applications. Processing every logical write as an independent physical storage operation can introduce significant overhead and reduce the efficiency of the storage subsystem.

A naive write-through implementation may perform one physical storage operation for every logical application request. This can increase the number of device operations and limit the ability of the storage system to exploit parallelism.

The project addresses this problem by investigating techniques that can reduce unnecessary physical operations while maintaining write-through persistence semantics.

The main techniques investigated are:

- Write coalescing
- Batched write submission
- Multiple NVMe queue pairs
- Concurrent application request processing
- Workload-aware performance evaluation

---

## 3. Objectives

The primary objectives of the project are:

1. Develop a Linux-based NVMe storage-system simulator using C++17.
2. Implement a baseline synchronous write-through path.
3. Implement an optimized write-through path.
4. Reduce redundant physical storage operations through write coalescing.
5. Improve storage parallelism using multiple queue pairs.
6. Implement batched request processing.
7. Generate configurable storage workloads.
8. Measure and compare system performance.
9. Study the effect of workload distribution and caching techniques.
10. Extend the project toward Linux device-driver and system-programming concepts.

---

## 4. Project Scope

The project currently focuses on the simulation and evaluation of an NVMe-style write-through storage path.

### In Scope

- Linux system programming
- C++17 implementation
- NVMe queue-pair modeling
- Write-through caching
- Write coalescing
- Batch processing
- Multi-queue request dispatch
- Concurrent request processing
- Workload generation
- Uniform and Zipfian workloads
- Storage performance measurement
- Latency analysis
- Throughput measurement
- IOPS measurement
- Queue utilization measurement
- File-backed storage using Linux system calls
- Performance visualization
- Future integration with a Linux device-driver interface

### Out of Scope

The current implementation does not attempt to reproduce the complete behavior of physical NVMe hardware.

The simulator does not currently implement:

- A complete production-grade NVMe kernel driver
- Real PCIe NVMe controller hardware
- Full NVMe protocol compliance
- Hardware firmware behavior
- Production storage management
- Enterprise-grade fault tolerance

These areas may be considered as future extensions.

---

## 5. Key Features

### Write-Through Cache

Every logical write request is treated as requiring persistence to the backing storage system.

### Write Coalescing

Multiple writes targeting overlapping or nearby storage regions can be combined to reduce the number of physical storage operations.

### Batched Submission

Write requests can be accumulated and submitted in batches instead of processing every request independently.

### Multi-Queue Processing

The optimized implementation supports multiple NVMe-style queue pairs, allowing requests to be distributed across multiple queues.

### Concurrent Processing

Multiple application threads can generate and process storage requests concurrently.

### Workload Generation

The simulator supports configurable workloads, including Zipfian request distributions that model workloads with non-uniform access patterns.

### Performance Metrics

The system records:

- Average latency
- p50 latency
- p95 latency
- p99 latency
- Maximum latency
- Application throughput
- Physical throughput
- Application IOPS
- Physical device IOPS
- Physical storage operations
- Queue utilization

---

## 6. Technology Stack

| Category | Technology |
|---|---|
| Operating System | Ubuntu Linux on WSL2 |
| Programming Language | C++17 |
| Compiler | GNU g++ |
| Build System | GNU Make |
| Version Control | Git |
| Concurrency | POSIX threads / C++ threading |
| Storage Interface | Linux system calls |
| Storage Access | `pwrite()` / `O_DIRECT` |
| Memory Management | POSIX aligned allocation |
| Visualization | Python / Matplotlib |
| Development Environment | WSL2 + Ubuntu |

---

## 7. System Overview

The current system can be represented as:

```text
Application Workload
        |
        v
+---------------------------+
| Write-Through Cache       |
|                           |
| Baseline / Optimized Path |
+-------------+-------------+
              |
              v
+---------------------------+
| NVMe Device Model         |
|                           |
| Multiple Queue Pairs      |
+-------------+-------------+
              |
              v
+---------------------------+
| Linux Storage Interface   |
|                           |
| pwrite() + O_DIRECT      |
+-------------+-------------+
              |
              v
      Backing Storage File
# 8. Expected Outcome

The expected outcome is a working Linux-based storage-system simulator that demonstrates how different write-processing techniques affect storage performance.

The project will provide measurable comparisons between the baseline and optimized implementations using controlled workloads.

The final system is also intended to demonstrate practical understanding of:
- Linux system programming
- C++ programming
- Operating-system concepts
- Storage systems
- Device-driver concepts
- Concurrent programming
- Performance analysis
- Software engineering and version control

# 9. Current Prototype Status

The current prototype successfully builds and runs under Ubuntu on WSL2.

The implementation currently supports:
- 20,000 logical write requests
- 50,000-address workload space
- 8 queue pairs
- Zipfian workload distribution
- Write-through caching
- Write coalescing
- Batch processing
- Multi-queue dispatch
- Performance measurement 
- CSV result generation 
 
The current prototype provides a functional foundation for further development.
 
# 10. Future Development Direction 
 
The project will be progressively extended to strengthen its Linux system-programming and device-driver aspects.
 
Planned areas include:
1. Improved storage request management 
2. More comprehensive testing 
3. Performance optimization 
4. Linux device-driver interface exploration 
5. Additional system-level interfaces 
6. Better error handling 
7. Improved documentation 
8. Automated benchmarking 
9. Extended performance visualization 
 
The final implementation will integrate the developed components into a coherent Linux storage-system project.
 
# 11. Applications 
 
the concepts demonstrated by this project are relevant to:
dStorage-system research
dNVMe performance analysis
dOperating-system experimentation
dCache design
dHigh-performance storage applications
dSystem programming education
dPerformance engineering
dStorage workload analysis

# 12. Development Environment # The project is developed using:
wWindows Host | WSL2 | Ubuntu Linux | +-- GCC / G++ | +-- GNU Make | +-- Git | +-- C++17 | +-- Linux system calls | +-- POSIX threading This environment allows the project to use Linux development tools and system-programming interfaces without requiring a separate virtual machine.
