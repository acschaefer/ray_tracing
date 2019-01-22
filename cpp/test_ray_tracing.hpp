#ifndef TEST_RAY_TRACING_H_
#define TEST_RAY_TRACING_H_ TEST_RAY_TRACING_H_

#include "grid_map.hpp"
#include <algorithm>
#include <array>
#include <cmath>


template <int N>
static double norm(std::array<double, N> const & a) {
    double sqsum = 0.0;
    for (int i = 0; i < N; ++i) {
        sqsum += a[i] * a[i];
    }
    return sqrt(sqsum);
}


template <int N>
void trace_ray_num(
        std::array<double, N> const & start, 
        std::array<double, N> const & end, 
        GridMap<N> & map) {
    std::array<double, N> v(end);
    for (int i = 0; i < N; ++i) {
        v[i] -= start[i];
    }

    std::array<int, N> const & shape = map.get_shape();
    std::array<double, N> const & size = map.get_size();
    double const min_size = *std::min_element(size.begin(), size.end());
    double const dt = min_size / norm<N>(v) / 1000;

    std::array<int, N> index;
    std::array<int, N> last_index;
    last_index.fill(-1);
    for (double t = 0.0; t < 1.0; t += dt) {
        for (int i = 0; i < N; ++i) {
            index[i] = std::floor((start[i] + t * v[i]) / size[i]);
            if (index[i] < 0 || index[i] >= map.get_shape()[i]) {
                index[0] = -1;
                break;
            }
        }
        if (index[0] >= 0 && index != last_index) {
            map.get_miss(index)++;
            last_index = index;
        }
    }

    for (int i = 0; i < N; ++i) {
        index[i] = std::floor(end[i] / size[i]);
        if (index[i] < 0 || index[i] >= shape[i]) {
            return;
        }
    }

    map.get_miss(index)--;
    map.get_hit(index)++;
}


# endif
