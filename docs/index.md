---
title: RTRBench
---

#  RTRBench

Real-Time Robotics Benchmark (RTRBench) is a collection of real-time robotic
kernels. The kernels span the entire software pipeline of most autonomous
robots ([Listing of Benchmarks](./benchmarks.md)). RTRBench is published in ISPASS 2022
[[Paper](https://cmu.box.com/s/pv22s6c4jn9oje951bbb6g3skvbg3ls8)]
[[Slides](https://cmu.box.com/s/jz98v4jvihqys53chzdhrlvt6lu8m8oy)]
[[Talk](https://cmu.box.com/s/0ixsyfb611g9m3uqsku7mohi73k4xup6)].

### Table of Contents

1. [Getting Started](#getting-started)
   - [Requirements](#requirements)
   - [Build](#build)
   - [Run](#run)
2. [More Details](#more-details)
   - [Build Options](#build-options)
   - [Run Scripts](#run-scripts)
   - [Inputsets](#inputsets)

   
## Getting Started

The benchmark suite can be downloaded from GitHub using:

```
$ git clone https://github.com/cmu-roboarch/rtrbench.git
```

### Requirements

The benchmark suite has been tested under Ubuntu 18.04 with Linux Kernel 4.15,
and compiled with GCC 11.1.0.  However, it can be used with a variety of other
operating systems and compilers. As the benchmarks are coded with C++17, the
minimum required GCC version is 8.0 (Clang: 5.0; Visual Studio: 15.8).

### Build
Every kernel has its own Makefile. To build a kernel, issue the following
command:
```
$ make
```

### Run
To run every kernel, simply issue the following command:
```
$ ./BINARY_NAME ARGS [ARGS]
```

Some of the arguments are required (e.g., input map, etc), and some of them are
optional. To see the description of the arguments, run the following command:
```
$ ./BINARY_NAME --help
```

## More Details

### Build Options
To build the kernel for the highest performance, use the following command.
This way, the pieces of the code that are written for the purpose of
verification will be omitted.
```
$ make performance
```

To build the kernels for debugging them, use the following command:
```
$ make debug
```

Use `build.sh` and `clean.sh` scripts to build and clean *all* of the kernels.

### Run Scripts
Every kernel is equipped with `run_all.sh` script. The script runs the
corresponding kernel with different execution/configuration parameters.  Put
your desired parameters into parameter arrays (started with `ALL_`), and issue
the following command. The script will run different experiments, one per
configuration.
```
$ ./run_all.sh
```

### Inputsets
Every kernel is equipped with its own inputset. `README.md` files in the
kernels' directories describe how other inputsets can be used/generated for
evaluation.
