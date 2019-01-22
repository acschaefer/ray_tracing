#include "grid_map.hpp"
#include "ray_tracing.hpp"
#include <array>
#include <vector>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>


template <typename T, int N>
void register_vector_array(pybind11::module & module) {
    std::stringstream name;
    name << "va" << typeid(T).name() << N;
    pybind11::class_<std::vector<std::array<T, N>>>(
            module, 
            name.str().c_str(),
            pybind11::buffer_protocol())
        .def(pybind11::init([](pybind11::buffer buffer) {
            pybind11::buffer_info info = buffer.request();
            
            if (info.format != pybind11::format_descriptor<T>::format()) {
                std::stringstream msg;
                msg << "Input array must be of type " 
                    << typeid(T).name() << ".";
                throw std::runtime_error(msg.str());
            }

            if (info.ndim != 2) {
                throw std::runtime_error("Input array must be 2D.");
            }

            if (info.shape[1] != N) {
                std::stringstream msg;
                msg << "Input array must be of size Nx" << N << ".";
                throw std::runtime_error(msg.str());
            }

            std::vector<std::array<T, N>> vector((size_t)info.shape[0]);
            std::vector<ssize_t> steps;
            for (int i = 0; i < 2; ++i) {
                steps.push_back(
                    info.strides[i] / static_cast<ssize_t>(sizeof(T)));
            }

            T * data = static_cast<T *>(info.ptr); 
            for (int i = 0; i < info.shape[0]; ++i) {
                for (int j = 0; j < info.shape[1]; ++j) {
                    vector[i][j] = data[i * steps[0] + j * steps[1]];
                }
            }
            return vector;
        }))
        .def_buffer([](std::vector<std::array<T, N>> & vector)
                -> pybind11::buffer_info {
            return pybind11::buffer_info(
                vector.data(),
                sizeof(T),
                pybind11::format_descriptor<T>::format(),
                2,
                {vector.size(), (size_t)N},
                {sizeof(T) * N, sizeof(T)});
        });
}


template <int N>
void register_map(pybind11::module & module) {
    std::stringstream name;
    name << "gridmap" << N;
    pybind11::class_<GridMap<N>>(module, name.str().c_str())
        .def(pybind11::init<std::array<int, N> const &, 
            std::array<double, N> const &>())
        .def_property("hits",
            &GridMap<N>::get_hits, &GridMap<N>::set_hits)
        .def_property("misses",
            &GridMap<N>::get_misses, &GridMap<N>::set_misses);
}


template <int N>
void register_function(pybind11::module & module) {
    std::stringstream name;
    name << "trace" << N << "d";
    std::stringstream description;
    description << "Amanatides-Woo ray tracing in " << N << "D";
    module.def(name.str().c_str(), &trace_rays<N>, description.str().c_str(),
        pybind11::arg("start"), pybind11::arg("end"), pybind11::arg("map"));
}


template <int N>
void register_all(pybind11::module & module) {
    register_vector_array<double, N>(module);
    register_vector_array<int, N>(module);
    register_map<N>(module);
    register_function<N>(module);
}


PYBIND11_MAKE_OPAQUE(std::vector<int>);
PYBIND11_MAKE_OPAQUE(std::vector<std::array<double, 1>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::array<int, 1>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::array<double, 2>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::array<int, 2>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::array<double, 3>>);
PYBIND11_MAKE_OPAQUE(std::vector<std::array<int, 3>>);


PYBIND11_PLUGIN(ray_tracing_python) {
    pybind11::module module("ray_tracing_python", "Amanatides-Woo ray tracing");

    pybind11::class_<std::vector<int>>(
            module, 
            "vi", 
            pybind11::buffer_protocol())
        .def(pybind11::init([](pybind11::buffer buffer) {
            pybind11::buffer_info info = buffer.request();
            
            if (info.format != pybind11::format_descriptor<int>::format()) {
                throw std::runtime_error(
                    "Input array must be of type \"int\".");
            }

            if (info.ndim != 1) {
                throw std::runtime_error("Input array must be 1D.");
            }

            std::vector<int> vector;
            vector.reserve((size_t)info.shape[0]);
            ssize_t step = info.strides[0] / static_cast<ssize_t>(sizeof(int));
            int * data = static_cast<int *>(info.ptr);
            int * end = data + info.shape[0] * step;
            for (int * data = static_cast<int *>(info.ptr); 
                    data != end; 
                    data += step) {
                vector.push_back(*data);
            }
            return vector;
        }))
        .def_buffer([](std::vector<int> & vector) -> pybind11::buffer_info {
            return pybind11::buffer_info(
                vector.data(),
                sizeof(int),
                pybind11::format_descriptor<int>::format(),
                1,
                {vector.size()},
                {sizeof(int)});
        });

    register_all<1>(module);
    register_all<2>(module);
    register_all<3>(module);

    return module.ptr();
}
