# RTRBench

Real-Time Robotics Benchmark (RTRBench) is a collection of 16 robotic kernels.
The kernels span the entire software pipeline of most autonomous robots.
Complete information on the benchmark suite can be found on [this
page](https://cmu-roboarch.github.io/rtrbench/).

## Highlights
- **Performance:** All kernels are implemented with performance as the main
  objective.
- **Easy to Simulate:** All kernels are easy to simulate with
  microarchitectural simulators (no Python runtime, no inter-process
communications, etc). The kernels communicate the regions of interest with the
simulator.
- **No Dependency Hell:** The kernels use few external libraries and all of
  them are included in the repository (no Boost, no Eigen, etc).

## Requirements
RTRBench uses C++17. The following minimum versions are required to build the
benchmarks:
- GCC 8.0
- Clang 5.0
- Visual Studio 15.8

## Build
Every kernel can be built using the following command:
```
$ make
```

### Build for Performance
To build the kernel for the highest performance, use the following command:
```
$ make performance
```

This way, the pieces of the code that are written for the purpose of
verification will be omitted.

### Build for Debug
To build the kernels for debugging them, use the following command:
```
$ make debug
```

### Build & Clean All

Use `build.sh` and `clean.sh` scripts to build and clean *all* of the kernels.

## Run

To run every kernel, simply issue the following command:
```
$ ./BINARY_NAME ARGS [ARGS]
```

Some of the arguments are required (e.g., input map, etc), and some of them are
optional. To see the description of the arguments, run the following command:
```
$ ./BINARY_NAME --help
```

Alternatively, you can put your desired parameters into the `run_all.sh`
scripts available in the kernels' directories, and simply issue the following
command:
```
$ ./run_all.sh
```

## License
```
Copyright (c) 2022 Carnegie Mellon University

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
