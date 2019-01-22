#ifndef RAYTRACING_H_
#define RAYTRACING_H_ RAYTRACING_H_

#include "grid_map.hpp"
#include <algorithm>
#include <array>
#include <exception>
#include <mutex>
#include <sstream>
#include <thread>
#include <vector>


template <int N>
void trace_ray(
        std::array<double, N> const & start, 
        std::array<double, N> const & end,
        GridMap<N> & map) {
    std::array<double, N> const & u(start);
    std::array<double, N> v(end);
    for (int i = 0; i < N; ++i) {
        v[i] -= u[i];
    }

    std::array<int, N> const & shape = map.get_shape();
    std::array<double, N> const & size = map.get_size();

    std::vector<std::array<double, N>> t_border(2, std::array<double, N>());
    for (int i = 0; i < N; ++i) {
        t_border[0][i] = -u[i] / v[i];
        t_border[1][i] = (shape[i] * size[i] - u[i]) / v[i];
        if (t_border[0][i] > t_border[1][i]) {
            std::swap(t_border[0][i], t_border[1][i]);
        }
    }

    double t = std::max(
        0.0, *std::max_element(t_border[0].begin(), t_border[0].end()));
    if (t >= 1.0) {
        return;
    } else if (*std::min_element(t_border[1].begin(), t_border[1].end()) < t) {
        return;
    }
    t = std::max(0.0, t);

    std::array<int, N> index;
    for (int i = 0; i < N; ++i) {
        index[i] = std::min(shape[i] - 1, (int)((u[i] + t * v[i]) / size[i]));
    }
    map.get_miss(index)++;

    std::array<int, N> step;
    for (int i = 0; i < N; ++i) {
        if (v[i] >= 0.0) {
            step[i] = 1;
        } else {
            step[i] = -1;
        }
    }

    std::array<double, N> t_max;
    int next_index;
    for (int i = 0; i < N; ++i) {
        next_index = index[i] + (step[i] > 0);
        t_max[i] = (next_index * size[i] - u[i]) / v[i];
    }

    while ((t = *std::min_element(t_max.begin(), t_max.end())) < 1.0) {
        for (int i = 0; i < N; ++i) {
            if (t_max[i] == t) {
                index[i] += step[i];

                if (index[i] < 0 || index[i] >= shape[i]) {
                    return;
                }

                next_index = index[i] + (step[i] > 0);
                t_max[i] = (next_index * size[i] - u[i]) / v[i];
            }
        }

        map.get_miss(index)++;
    }

    map.get_miss(index)--;
    map.get_hit(index)++;
}


std::mutex map_mutex;


template <int N>
static void trace_ray_thread(
        int worker,
        std::vector<std::array<double, N>> const & start,
        std::vector<std::array<double, N>> const & end,
        GridMap<N> & map) {
    int const workers = std::thread::hardware_concurrency();
    GridMap<N> worker_map(map.get_shape(), map.get_size());
    for (int i = worker; i < (int)start.size(); i += workers) {
        trace_ray<N>(start[i], end[i], worker_map);
    }

    map_mutex.lock();
    map += worker_map;
    map_mutex.unlock();
}


template<int N>
void trace_rays(
        std::vector<std::array<double, N>> const & start,
        std::vector<std::array<double, N>> const & end,
        GridMap<N> & map) {
    if (start.size() != end.size()) {
        throw std::invalid_argument(
            "Input arguments \"start\" and \"end\" must be of equal size.");
    }

    std::vector<std::thread> threads;
    int const workers = std::min<int>(
        start.size(), std::thread::hardware_concurrency());
    for (int i = 0; i < workers; ++i) {
        threads.push_back(std::thread(
            trace_ray_thread<N>, 
            i,
            std::cref(start), 
            std::cref(end),
            std::ref(map)));
    }

    for (int i = 0; i < workers; ++i) {
        threads[i].join();
    }
}


#endif
