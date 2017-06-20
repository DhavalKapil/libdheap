# libdheap

A shared (dynamic) library that can be transparently injected into different processes to detect memory corruption in glibc heap.

It works by intercepting and wrapping over libc's malloc() and free() while maintaining information about various chunks in an intermediate data storage (also on the heap). Also, canaries are added before and after heap chunks to detect overflows. 

```
+------------------+                               +---------------------+
|                  |       malloc(), free()        |                     |
|      User        | ----------------------------> |      libdheap       |
|     Process      | <---------------------------- | (injected library)  |
|                  |         intercepted           |                     |
+------------------+                               +---------------------+
       | ^                                                   | ^
       | |                               __libc_**           | |
       | |                          +------------------------+ |
       | |                          | +------------------------+
       | |                          | |
       | |                          v |
       | |                   +---------------+               +------------+-----------+
       | |  printf(), etc.   |               |    chunks     |            |           |
       | +------------------ |     glibc     | ------------> |  user      | libdheap  |
       +-------------------> |    library    | <------------ |  data      |   data    |
                             |               |               |            |           |
                             +---------------+               +------------+-----------+
                                    ^ |                             Heap Memory
                                    | |
                                    | | brk(), mmap()
                                    | |
     -------------------------------------------------------------------------
                                    | |                Operating System
                                    | v
                             +---------------+
                             |               |
                             |    kernel     |
                             |               |
                             +---------------+
```

## Features

* Runs directly on compiled code. Ideal for detecting errors in programs whose source code is unavailable
* Detects **invalid frees** including double frees
* Detects if malloc returns a **chunk which overlaps** with an already allocated chunk
* Detects any kind of **buffer based overflow or underflow**. This also detects many 'use after free' vulnerabilities
* Dynamic library, can be **attached to any process** (provided required permissions are available)
* Displays the **stack trace** (the function call history) on detecting any of the above errors

## Installation

This library is _not_ portable and works only with glibc.

To install, clone this repository and `cd` to it:

```sh
git clone https://github.com/DhavalKapil/libdheap
```

Run `make`:

```sh
make
```

The shared library will be generated: `libdheap.so`

## Usage

To run any program with `libdheap`, load the library using `LD_PRELOAD` environment variable.

```sh
LD_PRELOAD=/path/to/libdheap.so gedit
```

`libdheap` will output any error/log to standard error by default. You might want to redirect the output to some external file.

```sh
[LIBDHEAP LOG] : Freeing non allocated chunk!
[LIBDHEAP LOG] : Printing Stack Trace ====>
[LIBDHEAP LOG] :    0x400604
[LIBDHEAP LOG] :    0x2b3b8016ff45
[LIBDHEAP LOG] : <==== End of Stack Trace
```

`libdheap` allows setting two configuration options (through environment variables) as follow:

1. `LIBDHEAP_DEBUG`: If 1, will output debugging statements along with errors and logs.
2. `LIBDHEAP_EXIT_ON_ERROR`: If 1, will exit the instant it detects any memory corruption error.

By default, both are set to 0. Use the following command to configure:

```sh
LD_PRELOAD=/path/to/libdheap.so LIBDHEAP_DEBUG=1 gedit
```

Note: If debugging is enabled, it is advised to redirect output to an external file (`libdheap` outputs a **lot** of things). Also, this library is not developed for using in production, since it slows the application by approximately 5 times.

## Implementation details

* Uses a custom stack tracer (by jumping around the memory using the frame pointer). Existing stack tracers don't work as they are themselves dependent upon 'malloc', 'free', etc.
* Uses AVL trees for storing chunks as non overlapping sorted intervals.

## Contribution

Feel free to [file issues](https://github.com/DhavalKapil/libdheap/issues) and submit [pull requests](https://github.com/DhavalKapil/libdheap/pulls) – contributions are welcome.

## License

libdheap is licensed under the [MIT license](https://dhaval.mit-license.org/2017/license.txt).