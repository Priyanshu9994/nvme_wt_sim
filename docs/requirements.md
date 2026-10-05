# Project Requirements

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


---

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


---

# Development Plan

## 1. Development Objective

The project will be developed incrementally from the existing NVMe write-through simulator into a complete Linux system-programming project.

Development will focus on correctness first, followed by architecture improvements, device-driver integration, testing, benchmarking, and documentation.

---

## 2. Development Strategy

The implementation will follow these principles:

1. Preserve the currently working prototype.
2. Make small, independently testable changes.
3. Compile and test after significant changes.
4. Maintain Git history throughout development.
5. Measure performance before and after optimization.
6. Document important design and implementation decisions.
7. Avoid introducing unnecessary dependencies.
8. Keep userspace and kernel/interface components clearly separated.

---

## 3. Development Milestones

### Milestone 1 — Repository and Documentation

Tasks:

- Establish Git repository.
- Configure `.gitignore`.
- Preserve the existing working prototype.
- Document the project objective.
- Document the current baseline performance.
- Define functional and non-functional requirements.
- Define the development plan.

Deliverables:

- Source code
- README
- Project overview
- Requirements document
- Baseline performance document
- Development plan

---

### Milestone 2 — System Architecture

Tasks:

- Analyze existing C++ modules.
- Define module responsibilities.
- Define data flow between components.
- Document the storage request lifecycle.
- Design system architecture.
- Create class diagram.
- Create sequence diagram.
- Create state machine diagram where applicable.
- Define interfaces between major components.

Deliverables:

- Architecture documentation
- Architecture diagram
- Class diagram
- Sequence diagram
- State machine diagram
- Updated implementation plan

---

### Milestone 3 — Core System Implementation

Tasks:

- Refine workload generation.
- Refine write-through cache behavior.
- Improve request coalescing.
- Improve batch management.
- Improve queue-pair management.
- Improve concurrency handling.
- Improve error handling.
- Maintain performance metrics.

Deliverables:

- Updated C++ implementation
- Improved workload generator
- Improved cache and queue management
- Updated benchmark results

---

### Milestone 4 — Linux System Programming and Device Interface

Tasks:

- Analyze Linux device interfaces relevant to the project.
- Design the user-space/device interface.
- Develop the driver or kernel-interface component where supported by WSL2.
- Implement user-space communication with the interface.
- Document kernel/user-space responsibilities.
- Test the interface independently.

Deliverables:

- Driver/interface source code
- Build configuration
- Interface documentation
- User-space integration
- Driver/interface test results

---

### Milestone 5 — Testing and Performance Evaluation

Tasks:

- Develop unit tests for core components.
- Develop integration tests.
- Test error-handling paths.
- Test different workload distributions.
- Test different queue counts.
- Test different batch sizes.
- Test different workload sizes.
- Compare baseline and optimized implementations.
- Analyze latency and throughput.
- Investigate performance regressions.

Deliverables:

- Test suite
- Test results
- Benchmark data
- Performance charts
- Performance analysis

---

### Milestone 6 — Final Integration

Tasks:

- Integrate all components.
- Resolve remaining defects.
- Review code quality.
- Validate build instructions.
- Validate reproducibility.
- Update README.
- Update architecture documentation.
- Finalize diagrams.
- Finalize benchmark results.
- Prepare demonstration workflow.

Deliverables:

- Complete working project
- Final documentation
- Final test results
- Final performance results
- Demonstration instructions

---

## 4. Implementation Order

The implementation will generally follow this order:

```text
Existing Working Prototype
          |
          v
Requirements & Architecture
          |
          v
Workload Generation
          |
          v
Write-Through Cache
          |
          v
Coalescing
          |
          v
Batch Processing
          |
          v
Multi-Queue Processing
          |
          v
Linux Storage Interface
          |
          v
Device / Kernel Interface
          |
          v
Testing
          |
          v
Benchmarking
          |
          v
Final Integration

# 5. Testing Strategy

Testing will be performed at multiple levels.

## Unit Testing

Individual modules will be tested independently where practical.

**Examples:**
- Workload generation
- Request creation
- Coalescing logic
- Cache behavior
- Metrics calculations

## Integration Testing

Interactions between modules will be tested.

**Examples:**
- Workload generator → cache
- Cache → NVMe device model
- NVMe device model → storage backend
- Device/interface → userspace application

## System Testing
The complete application will be executed using representative workloads.

## Performance Testing
Performance will be measured using:
- Average latency
- p50 latency
- p95 latency
- p99 latency
-
 throughput
-
 IOPS
-
 Physical operation count
-
 Queue utilization 
 
# 6. Benchmark Plan 
The benchmark configuration will vary the following parameters:
| Parameter | Examples |
| --- | --- |
| Request count | 1,000 / 10,000 / 20,000 / larger |
| Address space | Small / medium / large |
| Queue count | 1 / 2 / 4 / 8 |
| Batch size | Different batch thresholds |
| Distribution | Uniform / Zipf |
| Zipf skew | Multiple skew values |
| Application threads | Different concurrency levels |
| Device latency | Multiple simulated latencies |
The purpose is to identify conditions under which each optimization is beneficial or introduces additional overhead.
 
# 7. Performance Evaluation Method 
every major performance change should be evaluated against a known baseline.
the evaluation process will be:
build -> run baseline -> run modified implementation -> collect metrics -> compare results -> analyze changes -> document findings.
pPerformance claims will be based on measured results rather than assumptions.
 
# 8. Version Control Strategy 
the project will use Git with main as the primary branch.
development will be organized into meaningful commits.
eexample commit sequence:
dInitial working NVMe write-through simulator
dAdd project documentation and baseline performance
dAdd project requirements and development plan
dAdd system architecture documentation
dAdd workload improvements
dImprove write coalescing
dAdd batch processing improvements
dAdd multi-queue processing
dAdd device interface
dAdd unit tests
dAdd integration tests
dAdd benchmark analysis
dImprove error handling
dUpdate documentation
dPrepare final release.
lLarge changes should be divided into smaller logical commits whenever practical.
 
# 9. Definition of Done 
a feature is considered complete when:
the implementation compiles successfully,
tRelevant tests pass,
tError conditions are considered,
the feature is documented,
git contains the corresponding change,
tPerformance impact is measured when applicable,
nNo known regression is introduced into previously working functionality.
 
# 10. Risk Management 
risk | mitigation 
wsl2 kernel limitations | verify available kernel development capabilities before driver implementation 
pPerformance regression | maintain reproducible baseline benchmarks 
cConcurrency bugs | use controlled tests and thread-safe data structures 
storage errors | add explicit error handling 
difficult driver integration | keep driver/interface component modular 
lLarge performance variance | use fixed seeds and repeated benchmark runs 
build failures | maintain documented dependencies and build commands 
scope expansion | prioritize required functionality before optional features.
n11. Final Deliverables The completed repository should contain:
c++ source code,
linux system-programming implementation,
device-driver/interface component,
build system,
tests,
benchmark workloads,
benchmark results,
pperformance charts,
a rchitecture documentation,UML diagrams,usage documentation,git history,final project report.
n12. Current Development Status The following items have already been completed:
eXisting C++ prototype,Linux/WSL2 build environment,Makefile-based build,Baseline implementation,Optimized write-through implementation,Workload generation,Performance measurement,Csv result generation,GIt repository initialization,Initial project documentation,Baseline performance documentation,Project requirements,Development plan.The next implementation focus is system architecture and detailed component design.
