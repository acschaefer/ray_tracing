#define CATCH_CONFIG_MAIN
#include "ray_tracing.hpp"
#include "test_ray_tracing.hpp"
#include <array>
#include <catch2/catch.hpp>
#include <random>
#include <vector>


template<int N>
void random_test() {
    std::default_random_engine gen;
    std::uniform_real_distribution<double> point_dist(-10.0, 110.0);
    std::uniform_real_distribution<double> extent_dist(1.0, 100.0);
    std::uniform_real_distribution<double> size_dist(1.0, 10.0);

    std::array<double, N> start;
    std::array<double, N> end;
    std::array<int, N> shape;
    std::array<double, N> size;

    for (int i = 0; i < 100; ++i) {
        for (int d = 0; d < N; ++d) {
            start[d] = point_dist(gen);
            end[d] = point_dist(gen);
            size[d] = size_dist(gen);
            shape[d] = std::max(1, (int)(extent_dist(gen) / size[d]));
        }

        GridMap<N> map_gt(shape, size);
        trace_ray_num<N>(start, end, map_gt);

        GridMap<N> map(shape, size);
        trace_ray<N>(start, end, map);

        REQUIRE(map_gt == map);
    }
}


template<int N>
void random_test_parallel() {
    std::default_random_engine gen;
    std::uniform_real_distribution<double> point_dist(-10.0, 110.0);
    std::uniform_real_distribution<double> extent_dist(1.0, 100.0);
    std::uniform_real_distribution<double> size_dist(1.0, 10.0);

    int const rays = 1000;
    std::vector<std::array<double, N>> start(rays);
    std::vector<std::array<double, N>> end(rays);
    for (int i = 0; i < rays; ++i) {
        for (int d = 0; d < N; ++d) {
            start[i][d] = point_dist(gen);
            end[i][d] = point_dist(gen);
        }
    }

    std::array<int, N> shape;
    std::array<double, N> size;
    for (int d = 0; d < N; ++d) {
        size[d] = size_dist(gen);
        shape[d] = std::max(1, (int)(extent_dist(gen) / size[d]));
    }

    GridMap<N> map_sequential(shape, size);
    for (int i = 0; i < rays; ++i) {
        trace_ray<N>(start[i], end[i], map_sequential);
    }

    GridMap<N> map_parallel(shape, size);
    trace_rays<N>(start, end, map_parallel);

    REQUIRE(map_sequential == map_parallel);
}


TEST_CASE("ray penetrates grid (1D, +x)", "[1D]") {
    std::array<double, 1> start = {-1.23};
    std::array<double, 1> end = {6.78};
    std::array<int, 1> shape = {5};
    std::array<double, 1> size = {0.45};
    
    GridMap<1> map_gt(shape, size);
    for (int i = 0; i < 5; ++i) {
        map_gt.get_miss({i})++;
    }

    GridMap<1> map_num(shape, size);
    trace_ray_num<1>(start, end, map_num);
    REQUIRE(map_gt == map_num);

    GridMap<1> map(shape, size);
    trace_ray<1>(start, end, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("ray penetrates grid (1D, -x)", "[1D]") {
    std::array<double, 1> start = {1.98};
    std::array<double, 1> end = {-6.71};
    std::array<int, 1> shape = {100};
    std::array<double, 1> size = {0.01};
    
    GridMap<1> map_gt(shape, size);
    for (int i = 0; i < 100; ++i) {
        map_gt.get_miss({i})++;
    }

    GridMap<1> map_num(shape, size);
    trace_ray_num<1>(start, end, map_num);
    REQUIRE(map_gt == map_num);

    GridMap<1> map(shape, size);
    trace_ray<1>(start, end, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("ray starts and ends in grid (1D, +x)", "[1D]") {
    std::array<double, 1> start = {0.671};
    std::array<double, 1> end = {0.985};
    std::array<int, 1> shape = {100};
    std::array<double, 1> size = {0.01};
    
    GridMap<1> map_gt(shape, size);
    int i;
    for (i = 67; i < 98; ++i) {
        map_gt.get_miss({i})++;
    }
    map_gt.get_hit({i})++;

    GridMap<1> map_num(shape, size);
    trace_ray_num<1>(start, end, map_num);
    REQUIRE(map_gt == map_num);

    GridMap<1> map(shape, size);
    trace_ray<1>(start, end, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("ray starts and ends in grid (1D, -x)", "[1D]") {
    std::array<double, 1> start = {0.985};
    std::array<double, 1> end = {0.671};
    std::array<int, 1> shape = {100};
    std::array<double, 1> size = {0.01};
    
    GridMap<1> map_gt(shape, size);
    int i;
    for (i = 98; i > 67; --i) {
        map_gt.get_miss({i})++;
    }
    map_gt.get_hit({i})++;

    GridMap<1> map_num(shape, size);
    trace_ray_num<1>(start, end, map_num);
    REQUIRE(map_gt == map_num);

    GridMap<1> map(shape, size);
    trace_ray<1>(start, end, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("ray starts in grid and ends outside (1D, +x)", "[1D]") {
    std::array<double, 1> start = {12.1};
    std::array<double, 1> end = {1009.7};
    std::array<int, 1> shape = {51};
    std::array<double, 1> size = {3.0};
    
    GridMap<1> map_gt(shape, size);
    for (int i = 4; i < 51; ++i) {
        map_gt.get_miss({i})++;
    }

    GridMap<1> map_num(shape, size);
    trace_ray_num<1>(start, end, map_num);
    REQUIRE(map_gt == map_num);

    GridMap<1> map(shape, size);
    trace_ray<1>(start, end, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("ray starts in grid and ends outside (1D, -x)", "[1D]") {
    std::array<double, 1> start = {109.7};
    std::array<double, 1> end = {-12.1};
    std::array<int, 1> shape = {51};
    std::array<double, 1> size = {3.0};
    
    GridMap<1> map_gt(shape, size);
    for (int i = 0; i < 37; ++i) {
        map_gt.get_miss({i})++;
    }

    GridMap<1> map_num(shape, size);
    trace_ray_num<1>(start, end, map_num);
    REQUIRE(map_gt == map_num);

    GridMap<1> map(shape, size);
    trace_ray<1>(start, end, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("random rays intersecting random grid (1D)", "[1D]") {
    random_test<1>();
}


TEST_CASE("random rays intersecting random grid (2D)", "[2D]") {
    random_test<2>();
}


TEST_CASE("identical start and end (2D) in cell", "[2D]") {
    std::array<double, 2> point = {0.23, 0.25};
    std::array<int, 2> shape = {50, 50};
    std::array<double, 2> size = {1.0, 1.0};

    GridMap<2> map_gt(shape, size);
    map_gt.get_hit({0, 0})++;
    
    GridMap<2> map(shape, size);
    trace_ray<2>(point, point, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("identical start and end (2D) on grid line", "[2D]") {
    std::array<double, 2> point = {0.0, 0.0};
    std::array<int, 2> shape = {50, 50};
    std::array<double, 2> size = {1.0 ,1.0};
    
    GridMap<2> map_gt(shape, size);
    map_gt.get_hit({0, 0})++;

    GridMap<2> map(shape, size);
    trace_ray<2>(point, point, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("identical start and end (2D) outside grid", "[2D]") {
    std::array<double, 2> point = {100.0, 100.0};
    std::array<int, 2> shape = {50, 50};
    std::array<double, 2> size = {1.0, 1.0};
    
    GridMap<2> map_gt(shape, size);

    GridMap<2> map(shape, size);
    trace_ray<2>(point, point, map);
    REQUIRE(map_gt == map);
}


TEST_CASE("random rays intersecting random grid (3D)", "[3D]") {
    random_test<3>();
}


TEST_CASE("random rays intersecting random grid (4D)", "[4D]") {
    random_test<4>();
}


TEST_CASE("random rays intersecting random grid (5D)", "[5D]") {
    random_test<5>();
}


TEST_CASE("parallel ray tracing (1D)", "[1D]") {
    int const rays = 1e5;
    std::vector<std::array<double, 1>> start(rays, {-1.23});
    std::vector<std::array<double, 1>> end(rays, {6.78});
    std::array<int, 1> shape = {5};
    std::array<double, 1> size = {0.45};
    
    GridMap<1> map_gt(shape, size);
    for (int i = 0; i < 5; ++i) {
        map_gt.get_miss({i}) = rays;
    }

    GridMap<1> map(shape, size);
    trace_rays<1>(start, end, map);

    REQUIRE(map_gt == map);
}


TEST_CASE("random parallel ray tracing (1D)", "[1D]") {
    random_test_parallel<1>();
}


TEST_CASE("random parallel ray tracing (2D)", "[2D]") {
    random_test_parallel<2>();
}


TEST_CASE("random parallel ray tracing (3D)", "[3D]") {
    random_test_parallel<3>();
}


TEST_CASE("random parallel ray tracing (4D)", "[4D]") {
    random_test_parallel<4>();
}


TEST_CASE("random parallel ray tracing (5D)", "[5D]") {
    random_test_parallel<5>();
}
