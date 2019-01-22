#!/usr/bin/env python

import numpy as np


class grid(object):
    def __init__(self, shape, size):
        if shape.ndim != 1 or size.ndim != 1:
            raise ValueError('Input arguments \"shape\" and \"size\" must have '
                'the same dimensionality.')
        if shape.shape != size.shape:
            raise ValueError('Input arguments \"shape\" and \"size\" must have '
                'the same shape.')
        if np.any(shape < 0) or np.any(size < 0.0):
            raise ValueError(
                'Input arguments \"shape\" and \"size\" must be positive.')
        self.shape = np.array(shape).astype(int)
        self.size = size

    def intersect(self, scan):
        d = scan.end - scan.start
        with np.errstate(divide='ignore'):
            tl = -scan.start / d
            tu = (self.shape * self.size - scan.start) / d
        tl[np.isnan(tl)] = 0.0
        tu[np.isnan(tu)] = 0.0
        tin = np.minimum(tl, tu)
        tout = np.maximum(tl, tu)
        return np.all(np.logical_and(tin < 1.0, tout > 0.0), axis=1)

    def index(self, point):
        if point.shape[1] != len(self.shape):
            raise ValueError(
                'Input argument \"index\" must be of shape Nx{}.'.format(
                    len(self.shape)))
        return (point // self.size).astype(int)

    def __eq__(self, other):
        return np.array_equal(self.shape, other.shape) \
            and np.array_equal(self.size, other.size)
