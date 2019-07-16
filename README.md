# ray_tracing

Fast grid-based ray tracing in C++ and Python

## What is `ray_tracing`?

This library implements ray tracing through a uniform grid according to the algorithm proposed by Amanatides and Woo:

> [Amanatides, John, and Andrew Woo.](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.42.3443&rep=rep1&type=pdf)<br/>
> [**A fast voxel traversal algorithm for ray tracing.**](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.42.3443&rep=rep1&type=pdf)<br/>
> [*Eurographics. Vol. 87. No. 3. 1987.*](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.42.3443&rep=rep1&type=pdf)

The `ray_tracing` library is written in C++.
In addition, it comes with Python bindings via [pybind11](https://pybind11.readthedocs.io/en/stable/).

## How to use `ray_tracing`?

1. Install the required dependencies Catch2, pytest, and pybind11.

   ```bash
    git clone https://github.com/catchorg/Catch2.git
    mkdir Catch2/build
    cd Catch2/build
    cmake ..
    make -j8
    sudo make install

    pip install pytest

    git clone https://github.com/pybind/pybind11.git
    mkdir pybind11/build
    cd pybind11/build
    cmake ..
    make -j8
    sudo make install
    ```

2. Download and compile the repository.
   
    ```bash
    git clone https://github.com/acschaefer/ray_tracing.git
    mkdir ray_tracing/build && cd ray_tracing/build
    cmake ..
    make -j8
    sudo make install
    ```

3. If you are in C++, use the function `trace_rays()` in the file [`ray_tracing.hpp`](cpp/ray_tracing.hpp) to find out by how many rays each grid cell is hit.
4. If you are in Python, install the library via the CMake file and use one of the functions `raytracing.trace1d`, `raytracing.trace2d`, and `raytracing.trace3d`, defined in [`raytracing.py`](python/raytracing/raytracing.py).

Also have a look at the test cases to verify your installation and to further familiarize yourself with the functions.
