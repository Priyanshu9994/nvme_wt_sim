# Linux Device Interface

## Overview

This directory contains a Linux kernel character-device module associated
with the NVMe Write-Through Caching Accelerator Simulator.

The module demonstrates:

- Linux kernel module development
- Character-device registration
- User-space/kernel-space communication
- `copy_to_user()` and `copy_from_user()`
- Kernel synchronization using a mutex
- Dynamic misc-device registration

This is a demonstration driver interface. It is not a production NVMe
hardware driver.

## Build

From the project root:

```bash
make -C driver
