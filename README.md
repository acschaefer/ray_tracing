# ray_tracing

Fast grid-based ray tracing in C++ and Python

## What is `ray_tracing`?

This library implements ray tracing through a uniform grid according to the algorithm proposed by Amanatides and Woo:

> Amanatides, John, and Andrew Woo.<br/>
> **A fast voxel traversal algorithm for ray tracing.**<br/>
> *Eurographics. Vol. 87. No. 3. 1987.*

The `ray_tracing` library is written in C++.
In addition, it comes with Python bindings via [pybind11](https://pybind11.readthedocs.io/en/stable/).

## How to use `ray_tracing`?

1. Compile the library using the top-level CMake file.
2. If you are in C++, use the function `trace_rays()` in the file [`ray_tracing.hpp`](cpp/ray_tracing.hpp) to find out by how many rays each grid cell is hit.
3. If you are in Python, install the library via the CMake file and use one of the functions `raytracing.trace1d`, `raytracing.trace2d`, and `raytracing.trace3d`, defined in [`raytracing.py`](python/raytracing/raytracing.py).

Also have a look at the test cases to verify your installation and to further familiarize yourself with the functions.
