# libdheap

A shared library that detects memory corruption in heaps dynamically.

## Features:

* libdheap runs directly on compiled code. This makes it ideal for detecting errors in programs whose source code is unavailable.
* Detects the following kinds of memory errors:
  1. 


## Installation:

## Usage:

## Implementation

* Uses a custom stack tracer (by jumping around the memory using the frame pointer). Existing stack tracers don't work as they are themselves dependent upon 'malloc', 'free', etc.
