# libdheap

A shared library that detects memory corruption in glibc heap dynamically.

## Features

* libdheap runs directly on compiled code. This makes it ideal for detecting errors in programs whose source code is unavailable.
* Detects invalid frees including double frees.
* Detects if malloc returns a chunk which overlaps with an already allocated chunk.
* Detects any kind of buffer based overflow or underflow. This also detects many 'use after free' vulnerabilities.
* Dynamic library, can be attached to any process (provided required permissions are available).
* Displays the stack trace (the function call history) on detecting any of the above errors.
* Optional debugging.

## Installation

This library is _not_ portable and works only with glibc.

To install, clone this repository and `cd` to it:

> https://github.com/DhavalKapil/libdheap

Run `make`:

> make

The shared library will be generated: `libdheap.so`

## Usage

To run any program with `libdheap`, load the library using `LD_PRELOAD` environment variable.

> LD_PRELOAD=/path/to/libdheap.so gedit

`libdheap` will output any error/log to standard error by default. You might want to redirect the output to some external file.

`libdheap` allows setting two configuration options (through environment variables) as follow:

1. `LIBDHEAP_DEBUG`: If 1, will output debugging statements along with errors and logs.
2. `LIBDHEAP_EXIT_ON_ERROR`: If 1, will exit the instant it detects any memory corruption error.

By default, both are set to 0. Use the following command to configure:

> LD_PRELOAD=/path/to/libdheap.so LIBDHEAP_DEBUG=1 gedit

Note: If debugging is enabled, it is advised to redirect output to an external file (`libdheap` outputs a **lot** of things). Also, this library is not developed for using in production, since it slows the application by approximately 5 times.

## Implementation

* Uses a custom stack tracer (by jumping around the memory using the frame pointer). Existing stack tracers don't work as they are themselves dependent upon 'malloc', 'free', etc.
