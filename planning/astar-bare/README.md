# A*

This directory provides an *abstract* implementation of A* graph search. The
other kernels, specifically `04.pp2d` and `05.pp3d`, use it as the base class
to have A* as the backend search engine.

**NOTE:** For parallel execution, please uncomment OpenMP primitives. Also, in
Makefile of the kernels, please uncomment the lines that add OpenMP flags to
the compilation process.
