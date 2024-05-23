# Simple Device Drivers for Testing Locking Mechanism in Linux

This repository contains two simple device drivers to demonstrate the application of semaphore and spinlocks in Linux.

## Directory Structure

- `sem_mut/` - Contains the source code and Makefile testing semaphore locks.
- `spin_lock/` - Contains the source code and Makefile for testing spinlocks.

## Getting Started

Below are the instructions to compile and load each driver into the Linux Kernel.

### Prerequisites

Ensure you have a Linux environment with the necessary tools installed (gcc, make, and appropriate kernel headers).

```bash
sudo apt update
sudo apt install build-essential
sudo apt install linux-headers-$(uname -r)
sudo apt install flex bison 
```

### Compiling the Drivers

To compile the drivers, navigate to the driver's directory and use the provided Makefile.

For `sem_mut`:
```bash
cd sem_mut
make
sudo insmod sem.ko

# to check the kernel messages 
sudo dmesg 

# to remove the module 
sudo rmmod sem
```

For `spin_lock`:
```bash
cd spin_lock
make
sudo insmod spinlk.ko

# to check the kernel messages 
sudo dmesg 

# to remove the module 
sudo rmmod spinlk
```