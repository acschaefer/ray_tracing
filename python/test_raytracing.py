#!/usr/bin/env python

import numpy as np
import pytest
import raytracing as rt


def test_ray_penetrates_grid_1d_pos_x():
    start = np.array([[-1.23]])
    end = np.array([[6.78]])
    shape = np.array([5])
    size = np.array([0.45])

    map_gt = rt.gridmap(shape, size)
    map_gt.misses[:5] = 1
    
    map = rt.gridmap(shape, size)
    rt.trace1d(start, end, map)

    assert(map_gt == map)


def test_ray_penetrates_grid_1d_neg_x():
    start = np.array([[1.98]])
    end = np.array([[-6.71]])
    shape = np.array([100])
    size = np.array([0.01])

    map_gt = rt.gridmap(shape, size)
    map_gt.misses = np.ones(100)

    map = rt.gridmap(shape, size)
    rt.trace1d(start, end, map)

    assert(map_gt == map)


def test_ray_starts_and_ends_in_grid_1d_pos_x():
    start = np.array([[0.671]])
    end = np.array([[0.985]])
    shape = np.array([100])
    size = np.array([0.01])

    map_gt = rt.gridmap(shape, size)
    map_gt.misses[67:98] = 1
    map_gt.hits[98] = 1

    map = rt.gridmap(shape, size)
    rt.trace1d(start, end, map)

    assert(map_gt == map)


def test_ray_starts_and_ends_in_grid_1d_neg_x():
    start = np.array([[0.985]])
    end = np.array([[0.671]])
    shape = np.array([100])
    size = np.array([0.01])

    map_gt = rt.gridmap(shape, size)
    map_gt.misses[98:67:-1] = 1
    map_gt.hits[67] = 1

    map = rt.gridmap(shape, size)
    rt.trace1d(start, end, map)

    assert(map_gt == map)


def test_ray_starts_in_grid_and_ends_outside_1d_pos_x():
    start = np.array([[12.1]])
    end = np.array([[1009.7]])
    shape = np.array([51])
    size = np.array([3.0])

    map_gt = rt.gridmap(shape, size)
    map_gt.misses[4:51] = 1

    map = rt.gridmap(shape, size)
    rt.trace1d(start, end, map)

    assert(map_gt == map)


def test_ray_starts_in_grid_and_ends_outside_1d_neg_x():
    start = np.array([[109.7]])
    end = np.array([[-12.1]])
    shape = np.array([51])
    size = np.array([3.0])

    map_gt = rt.gridmap(shape, size)
    map_gt.misses[:37] = 1

    map = rt.gridmap(shape, size)
    rt.trace1d(start, end, map)

    assert(map_gt == map)


def test_identical_start_and_end_2d_in_cell():
    point = np.array([[0.23, 0.25]])
    shape = np.array([50, 50])
    size = np.array([1.0, 1.0])

    map_gt = rt.gridmap(shape, size)
    map_gt.hits[0,0] = 1

    map = rt.gridmap(shape, size)
    rt.trace2d(point, point, map)

    assert(map_gt == map)


def test_identical_start_and_end_2d_on_grid_line():
    point = np.array([[0.0, 0.0]])
    shape = np.array([50, 50])
    size = np.array([1.0, 1.0])

    map_gt = rt.gridmap(shape, size)
    map_gt.hits[0,0] = 1

    map = rt.gridmap(shape, size)
    rt.trace2d(point, point, map)

    assert(map_gt == map)


def test_identical_start_and_end_2d_outside_grid():
    point = np.array([[100.0, 100.0]])
    shape = np.array([50, 50])
    size = np.array([1.0, 1.0])

    map_gt = rt.gridmap(shape, size)

    map = rt.gridmap(shape, size)
    rt.trace2d(point, point, map)
    
    assert(map_gt == map)


def test_parallel_ray_tracing_2d():
    start = np.array(
        [[-1.5, +1.5], 
         [+1.0, -2.0], 
         [+3.5, -1.0], 
         [+7.5, +1.0], 
         [+5.5, +4.5], 
         [-0.5, +2.0]])
    end = np.array(
        [[+2.5, +1.5],
         [+1.0, -0.5],
         [+3.5, +1.5],
         [+4.5, +0.5],
         [+5.5, +2.5],
         [+1.0, +3.5]])
    shape = np.array([6, 3])
    size = np.array([1, 1])

    map_gt = rt.gridmap(shape, size)
    
    map_gt.misses[:2,1] += 1
    map_gt.hits[2,1] += 1

    map_gt.misses[3,0] += 1
    map_gt.hits[3,1] += 1

    map_gt.misses[5,0] += 1
    map_gt.hits[4,0] += 1

    map_gt.hits[5,2] += 1

    map_gt.misses[0,2] += 1

    map = rt.gridmap(shape, size)
    rt.trace2d(start, end, map)

    assert(map_gt == map)

if __name__ == '__main__':
    pytest.main()
