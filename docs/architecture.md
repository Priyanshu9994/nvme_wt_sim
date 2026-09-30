# System Architecture

## 1. Architecture Overview

The NVMe Write-Through Caching Accelerator Simulator is organized as a layered system.

The application generates storage workloads, which are processed through a write-through cache layer before being submitted to an NVMe-style device model.

The device model manages multiple queue pairs and ultimately performs storage operations against a file-backed storage device using Linux system calls.

```text
+------------------------------------------------------+
|                  Application Layer                   |
|                                                      |
|              WorkloadGenerator                      |
|                    |                                 |
|                    v                                 |
|              WorkloadRequest                        |
+--------------------+---------------------------------+
                     |
                     v
+------------------------------------------------------+
|                 Cache Layer                         |
|                                                      |
|   +----------------------+  +---------------------+ |
|   | BaselineWriteThrough |  | OptimizedWrite     | |
|   | Cache                |  | ThroughCache        | |
|   +----------+-----------+  +----------+----------+ |
|              |                         |            |
|              |                         +-- Coalesce |
|              |                         +-- Batch    |
|              |                         +-- Threads  |
+--------------+-------------------------+------------+
               |
               v
+------------------------------------------------------+
|                 Device Layer                        |
|                                                      |
|                  NVMeDevice                         |
|                       |                              |
|          +------------+------------+                 |
|          |            |            |                 |
|          v            v            v                 |
|       Queue 0      Queue 1      Queue N              |
|          |            |            |                 |
|          +------------+------------+                 |
|                       |                              |
|                  WriteCommand                       |
+-----------------------+------------------------------+
                        |
                        v
+------------------------------------------------------+
|               Linux Storage Layer                   |
|                                                      |
|             O_DIRECT / pwrite()                    |
|                       |                              |
|                       v                              |
|                Backing Storage                      |
+------------------------------------------------------+

                 +------------------+
                 | MetricsCollector  |
                 |                  |
                 | Latency           |
                 | Throughput        |
                 | IOPS              |
                 | Queue utilization |
                 +------------------+


# Major Components

## 2.1 WorkloadGenerator

WorkloadGenerator creates storage requests according to the configured workload parameters.

**Responsibilities:**
- Generate logical write requests.
- Select storage addresses.
- Generate request data.
- Support uniform distribution.
- Support Zipfian distribution.
- Use a configurable random seed.

## 2.2 WorkloadRequest

WorkloadRequest represents a logical application write.

**Typical information includes:**
- Logical block address.
- Write data.
- Request metadata.

It forms the input to the write-through cache layer.

## 2.3 BaselineWriteThroughCache

BaselineWriteThroughCache represents the baseline storage path.

**Characteristics:**
- Synchronous processing.
- One queue.
- Queue depth of one.
- No request coalescing.
- No batching.

Each logical request is submitted as an individual physical storage operation.

## 2.4 OptimizedWriteThroughCache

OptimizedWriteThroughCache implements the optimized write path.

**Responsibilities include:**
- Accepting logical writes.
- Maintaining pending write entries.
- Coalescing suitable writes.
- Accumulating requests into batches.
- Flushing batches.
- Dispatching writes through the NVMe device.
- Coordinating background processing.

The optimized implementation uses concurrency to process requests across multiple queue pairs.

## 2.5 NVMeDevice

NVMeDevice represents the simulated NVMe storage device.

**Responsibilities:**
- Open the backing storage.
- Maintain NVMe queue pairs。
p-select queues for requests。
p-submit write commands。
p-provide access to device-level statistics。
p 

## 2.6 NVMeQueuePair

NVMeQueuePair represents an NVMe-style submission/completion queue pair. 

**Responsibilities:**

Accept write commands. 

Maintain queued work. 

Process requests through a worker thread. 

Track queue statistics. 
Model storage latency. 
Execute the underlying storage operation. \\*\*\*\*
display: inline;
display: inline;
display: inline;
display: inline;
display: inline;
display: inline;
