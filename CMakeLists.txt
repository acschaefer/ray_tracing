cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(ray_tracing LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra")

find_package(Catch2 REQUIRED)

set(PYTHON_EXECUTABLE /usr/bin/python)
find_package(pybind11 REQUIRED)

add_executable(test_ray_tracing cpp/test_ray_tracing.cpp)
target_link_libraries(test_ray_tracing Catch2::Catch2)

pybind11_add_module(ray_tracing_python cpp/ray_tracing_python.cpp)
