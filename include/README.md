This directory includes the implementation of miscellaneous functions RTRBench
uses. Excluding `rtrbench_utils.h`, all files are third-party.

[`nlohmann`](https://github.com/nlohmann/json) is used to operate on JSON
files.

[`parallel_hashmap`](https://greg7mdp.github.io/parallel-hashmap/) is used to
implement fast, (parallel) hashmaps.

[`args.h`](https://github.com/ElliotLockerman/cpp_args) is used to parse
command-line arguments.

[`glob.hpp`](https://github.com/p-ranav/glob) is used for finding files.

[`log.h`](https://github.com/s5z/zsim/blob/master/src/log.h) includes general
logging, warning, assertion, etc. routines.

`rtrbench_utils.h` implements some operations that are common among different
kernels of RTRBench (e.g., matrix multiplication, etc).

[`stb_image.h`](https://github.com/nothings/stb/blob/master/stb_image.h) is
used to read PNG images.

[`zsim_hooks.h`](https://github.com/s5z/zsim/blob/master/misc/hooks/zsim_hooks.h)
provides utility to communicate with [zsim](https://github.com/s5z/zsim)
simulator.
