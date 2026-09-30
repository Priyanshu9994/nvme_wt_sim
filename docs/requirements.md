# Project Requirements

## 1. Purpose

The NVMe Write-Through Caching Accelerator Simulator is intended to provide a Linux-based environment for studying write-through storage behavior, request coalescing, batching, multi-queue processing, and system-level performance.

The system will provide a baseline implementation and progressively introduce optimized mechanisms so their impact can be measured using reproducible workloads.

---

## 2. Functional Requirements

### FR-01: Workload Generation

The system shall generate configurable storage write workloads.

The workload generator shall support:

- Configurable number of requests
- Configurable address space
- Random seed
- Uniform distribution
- Zipfian distribution
- Configurable Zipf skew

### FR-02: Write-Through Processing

The system shall provide a write-through request path in which application writes are propagated to the backing storage layer.

### FR-03: Baseline Storage Path

The system shall provide a baseline synchronous write implementation using a single queue and queue depth of one.

### FR-04: Write Coalescing

The optimized implementation shall identify suitable write requests that can be combined into fewer physical storage operations.

### FR-05: Batched Submission

The system shall support grouping multiple write requests before submitting them to the storage layer.

### FR-06: Multi-Queue Processing

The system shall support multiple NVMe-style queue pairs for concurrent request processing.

### FR-07: Concurrent Processing

The system shall support multiple application threads generating storage requests.

### FR-08: Storage Interface

The system shall use Linux system-programming interfaces for interaction with the backing storage implementation.

### FR-09: Performance Measurement

The system shall collect:

- Average latency
- p50 latency
- p95 latency
- p99 latency
- Maximum latency
- Application throughput
- Physical throughput
- Application IOPS
- Physical IOPS
- Physical operation count
- Queue utilization

### FR-10: Result Generation

The system shall generate machine-readable benchmark results, including CSV output.

### FR-11: Comparison

The system shall provide a comparison between baseline and optimized execution.

### FR-12: Device-Driver Integration

The project shall provide a Linux device-driver/interface component where technically supported by the development environment.

The interface shall allow investigation of communication between user-space applications and a Linux kernel-level component.

---

## 3. Non-Functional Requirements

### NFR-01: Performance

The implementation should minimize unnecessary synchronization and processing overhead.

### NFR-02: Reliability

The system should handle invalid requests, storage errors, and resource-allocation failures safely.

### NFR-03: Reproducibility

Benchmark runs should support deterministic workload generation through configurable random seeds.

### NFR-04: Portability

The userspace components should compile on a standard Linux development environment supporting C++17.

### NFR-05: Maintainability

The implementation should use modular C++ classes with clear separation of responsibilities.

### NFR-06: Code Quality

The project should compile with warnings enabled and should avoid unnecessary compiler warnings.

### NFR-07: Documentation

Major components, interfaces, configuration parameters, experiments, and results shall be documented.

### NFR-08: Version Control

All significant development changes shall be tracked using Git.

### NFR-09: Testability

Core components should be independently testable where practical.

### NFR-10: Observability

The system should expose sufficient metrics and logs to investigate performance and correctness.

---

## 4. System Modules

The project is organized into the following major modules.

### 4.1 Workload Generator

Responsible for generating storage requests according to configurable workload parameters.

### 4.2 Write-Through Cache

Responsible for processing logical writes and maintaining write-through persistence behavior.

### 4.3 Coalescing Engine

Responsible for identifying requests that can be combined into fewer physical operations.

### 4.4 Batch Manager

Responsible for collecting and submitting groups of requests.

### 4.5 NVMe Device Model

Responsible for representing the NVMe-style storage interface and queue pairs.

### 4.6 Storage Backend

Responsible for interaction with the backing storage file using Linux system interfaces.

### 4.7 Metrics Engine

Responsible for collecting latency, throughput, IOPS, and queue-utilization measurements.

### 4.8 Result Generator

Responsible for producing CSV and other benchmark outputs.

### 4.9 Driver Interface

Responsible for the Linux kernel/user-space interface developed as part of the system-programming and device-driver component.

---

## 5. User Requirements

The user should be able to:

1. Build the project using the provided Makefile.
2. Configure workload parameters.
3. Execute the baseline implementation.
4. Execute the optimized implementation.
5. Generate benchmark results.
6. Compare baseline and optimized behavior.
7. Inspect generated performance data.
8. Reproduce experiments using a fixed random seed.

---

## 6. Constraints

The project is developed using:

- Ubuntu Linux running through WSL2
- C++17
- GNU Compiler Collection
- GNU Make
- Git
- Linux system-programming interfaces

The project is primarily a simulation and experimental system rather than a production NVMe implementation.

Actual kernel-driver capabilities may depend on the WSL2 kernel configuration and available kernel development facilities.

---

## 7. Expected Deliverables

The final project is expected to contain:

- Working C++ implementation
- Linux system-programming components
- Device-driver/interface component
- Build system
- Test suite
- Benchmark workloads
- Performance results
- Architecture documentation
- UML diagrams
- Source-code documentation
- Usage instructions
- Git history
- Final project report

---

## 8. Success Criteria

The project will be considered successful when:

1. The complete system builds successfully on the target Linux environment.
2. The baseline implementation executes correctly.
3. The optimized implementation executes correctly.
4. Workload generation is reproducible.
5. Performance metrics are collected correctly.
6. Baseline and optimized configurations can be compared.
7. Physical operation reduction can be measured.
8. Core functionality is covered by tests.
9. The Linux system-programming concepts are demonstrated.
10. The device-driver/interface component is implemented and documented to the extent supported by the development environment.
11. The complete project can be built and executed using documented instructions.
