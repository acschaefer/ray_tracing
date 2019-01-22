#!/usr/bin/env python

import numpy as np


class grid(object):
    def __init__(self, shape, size=np.ones((1,3))):
        self.shape = np.array(shape).astype(int)
        if self.shape.size != 3:
            raise ValueError('shape mismatch')
        if np.any(self.shape <= 0):
            raise ValueError('shape must be positive')
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
        if point.shape[1] != 3:
            raise ValueError('shape must be Nx3')
        return (point // self.size).astype(int)
