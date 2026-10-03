# Linux Driver Validation

## 1. Objective

The project includes a small Linux kernel character-device module that provides a kernel-space interface associated with the NVMe write-through simulator.

The module is intended to demonstrate Linux kernel module development and user-space/kernel-space communication.

It is not a production NVMe hardware driver.

---

## 2. Kernel Environment

The driver was built and tested using:

- WSL2
- Custom Microsoft WSL kernel
- Linux kernel: 6.6.123.2-microsoft-standard-WSL2+
- x86_64
- Ubuntu Linux

The kernel module was built externally using the Linux kernel build system.

---

## 3. Build Validation

The module was built using:
```bash
make -C /lib/modules/$(uname -r)/build M=$PWD modules
```
The resulting module was:
```text
nvme_wt_driver.ko
```
The generated module size during validation was approximately 258 KB.
---
## 4. Module Load Test
The driver was loaded using:
```bash
sudo insmod nvme_wt_driver.ko
```
Kernel messages confirmed:
```text
nvme_wt_sim: driver loaded
```
The module therefore initialized successfully.
---
## 5. Device Node Test
After loading the module:
```bash
gs -l /dev/nvme_wt_sim 
defaults to ls -l /dev/nvme_wt_sim, which confirms device node creation.
e.g.,
rw-rw-rw- 1 root root ... /dev/nvme_wt_sim 
does this confirm successful creation?
the answer is yes, as it shows the device node exists.
done!
done!
done!
done!
