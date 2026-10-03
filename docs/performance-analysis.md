# Performance Analysis

## 1. Benchmark Configuration

The benchmark compares:

1. Baseline synchronous write-through
2. Optimized coalesced, batched, and multi-queue write-through

### Configuration:

| Parameter           | Value |
|---------------------|--------:|
| Logical requests    | 20,000 |
| Address space       | 50,000 blocks |
| Queues              | 8 |
| Distribution        | Zipfian |
| Zipf skew           | 1.2 |
| Flush interval      | 200 us |
| Batch trigger       | 64 |
| Application threads | 8 |
| Random seed         | 42 |
| Simulated latency   | 60 us |

---

## 2. Latest Benchmark Results

### Baseline

| Metric                    | Result     |
|---------------------------|------------:|
| Physical operations       | 20,000     |
| Average latency           | 921.48 us  |
| p50 latency               | 740.95 us  |
| p95 latency               | 1842.14 us |
| p99 latency               | 2977.43 us |
| Maximum latency           | 8877.84 us |
| Physical throughput       | 31.18 MB/s |
| Application throughput    | 31.18 MB/s |
| Application IOPS          | 7982.31    |
| Physical device IOPS      | 7982.31    |
| Average queue utilization | 5.98%      |

---

### Latest Optimized Results

| Metric                    | Result     	|
|- ------------------------- |- ---------:|
total physical operations   	|	17,139 	|
total coalescing reduction 	|	14.31% 	|
total average latency     	|	1173.17 us 	|
p50 latency                	|	1000.34 us 	|
p95 latency                	|	2303.34 us 	|
p99 latency                	|	4145.33 us 	|
total maximum latency     	|	10672.82 us	|
total physical throughput   	|	22.74 MB/s	|
total application throughput	|	26.54 MB/s	|
total application IOPS      	|	6794.26   	y
total physical device IOPS	|	5822.34   	y
total average queue utilization	|	4.36%    \
