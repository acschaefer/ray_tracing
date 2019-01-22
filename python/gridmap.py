#!/usr/bin/env python

from grid import *
from mapviewer import *
import numpy as np
import ray_tracing.raytracing as rt


class gridmap(grid):
    def __init__(self, *args, **kwargs):
        super(gridmap, self).__init__(*args, **kwargs)
        self.hits = np.zeros(self.shape)
        self.misses = np.zeros(self.shape)

    def inside(self, index):
        return np.all(np.logical_and(
            0 <= index, index < np.resize(self.shape, index.shape)), axis=1)

    def incorporate(self, scan):
        misses = rt.trace3d(scan.start, scan.end, self.shape, self.size)

        hits = self.index(scan.end)
        hits = hits[
            np.all(np.logical_and(hits >= 0, hits < self.shape), axis=1),:]

        np.add.at(self.misses, misses, +1)
        np.add.at(self.misses, hits, -1)
        np.add.at(self.hits, hits, +1)

    def get_map(self):
        with np.errstate(divide='ignore'):
            return np.true_divide(self.hits, (self.hits + self.misses))

    def view(self):
        mapviewer(self.get_map())
