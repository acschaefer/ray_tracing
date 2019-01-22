#!/usr/bin/env python

from ..gridmap import *
import numpy as np
import pytest
import raytracing as rt


def test_ray_penetrates_grid_1d_pos_x():
    start = np.array([-1.23])
    end = np.array([6.78])
    shape = np.array([5])
    size = np.array([0.45])
    indices_gt = np.reshape(np.arange(5), (-1,1))
    indices = rt.trace1d(start, end, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_ray_penetrates_grid_1d_neg_x():
    start = np.array([1.98])
    end = np.array([-6.71])
    shape = np.array([100])
    size = np.array([0.01])
    indices_gt = np.reshape(np.arange(99, -1, -1), (-1,1))
    indices = rt.trace1d(start, end, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_ray_starts_and_ends_in_grid_1d_pos_x():
    start = np.array([0.671])
    end = np.array([0.985])
    shape = np.array([100])
    size = np.array([0.01])
    indices_gt = np.reshape(np.arange(67, 99), (-1,1))
    indices = rt.trace1d(start, end, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_ray_starts_and_ends_in_grid_1d_neg_x():
    start = np.array([0.985])
    end = np.array([0.671])
    shape = np.array([100])
    size = np.array([0.01])
    indices_gt = np.reshape(np.arange(98, 66, -1), (-1,1))
    indices = rt.trace1d(start, end, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_ray_starts_in_grid_and_ends_outside_1d_pos_x():
    start = np.array([12.1])
    end = np.array([1009.7])
    shape = np.array([51])
    size = np.array([3.0])
    indices_gt = np.reshape(np.arange(4, 51), (-1,1))
    indices = rt.trace1d(start, end, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_ray_starts_in_grid_and_ends_outside_1d_neg_x():
    start = np.array([1009.7])
    end = np.array([12.1])
    shape = np.array([51])
    size = np.array([3.0])
    indices_gt = np.reshape(np.arange(50, 3, -1), (-1,1))
    indices = rt.trace1d(start, end, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_identical_start_and_end_2d_in_cell():
    start = np.array([0.23] * 2)
    shape = np.array([50] * 2)
    size = np.array([1.0] * 2)
    indices_gt = np.array([[0,0]])
    indices = rt.trace2d(start, start, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_identical_start_and_end_2d_on_grid_line():
    start = np.array([0.0] * 2)
    shape = np.array([50] * 2)
    size = np.array([1.0] * 2)
    indices_gt = np.array([[0] * 2])
    indices = rt.trace2d(start, start, shape, size)
    assert(np.array_equal(indices_gt, indices))


def test_identical_start_and_end_2d_outside_grid():
    start = np.array([100.0] * 2)
    shape = np.array([50] * 2)
    size = np.array([1.0] * 2)
    assert(rt.trace2d(start, start, shape, size).size == 0)


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
    indices_gt = [
        np.array([[0,1], [1,1], [2,1]]),
        np.empty((0,2)),
        np.array([[3,0], [3,1]]),
        np.array([[5,0], [4,0]]),
        np.array([[5,2]]),
        np.array([[0,2]])]
    
    for s, e, i in zip(start, end, indices_gt):
        indices = rt.trace2d(s, e, shape, size)
        assert(np.array_equal(i, indices))

    indices = rt.trace2d(start, end, shape, size)
    assert(np.array_equal(
        np.sort(np.vstack(indices_gt), axis=0), np.sort(indices, axis=0)))


if __name__ == '__main__':
    test_ray_penetrates_grid_1d_pos_x()
    test_ray_penetrates_grid_1d_neg_x()
    test_ray_starts_and_ends_in_grid_1d_pos_x()
    test_ray_starts_and_ends_in_grid_1d_neg_x()
    test_ray_starts_in_grid_and_ends_outside_1d_pos_x()
    test_ray_starts_in_grid_and_ends_outside_1d_neg_x()
    test_identical_start_and_end_2d_in_cell()
    test_identical_start_and_end_2d_on_grid_line()
    test_identical_start_and_end_2d_outside_grid()
    test_parallel_ray_tracing_2d()
