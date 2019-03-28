#!/usr/bin/env python

from grid import *
import numpy as np


class gridmap(grid):
    def __init__(self, *args, **kwargs):
        super(gridmap, self).__init__(*args, **kwargs)
        self.hits = np.zeros(self.shape, dtype=np.int)
        self.misses = np.zeros(self.shape, dtype=np.int)

    def inside(self, index):
        return np.all(np.logical_and(
            0 <= index, index < np.resize(self.shape, index.shape)), axis=1)

    def reflectionmap(self):
        with np.errstate(divide='ignore', invalid='ignore'):
            return np.true_divide(self.hits, (self.hits + self.misses))

    def __eq__(self, other): 
        return super(gridmap, self).__eq__(other) \
            and np.array_equal(self.hits, other.hits) \
            and np.array_equal(self.misses, other.misses)
