#!/usr/bin/env python

import numpy as np
import ray_tracing_python as rtp


def trace1d(start, end, map):
    map1 = rtp.gridmap1(map.shape, map.size)
    rtp.trace1d(rtp.vad1(start), rtp.vad1(end), map1)
    map.hits += np.array(map1.hits, dtype=np.int, copy=False)
    map.misses += np.array(map1.misses, dtype=np.int, copy=False)


def trace2d(start, end, map):
    map2 = rtp.gridmap2(map.shape, map.size)
    rtp.trace2d(rtp.vad2(start), rtp.vad2(end), map2)
    map.hits += np.array(map2.hits, dtype=np.int, copy=False).reshape(map.shape)
    map.misses += np.array(map2.misses, dtype=np.int, copy=False).reshape(
        map.shape)


def trace3d(start, end, map):
    map3 = rtp.gridmap3(map.shape, map.size)
    rtp.trace3d(rtp.vad3(start), rtp.vad3(end), map3)
    map.hits += np.array(map3.hits, dtype=np.int, copy=False).reshape(map.shape)
    map.misses += np.array(map3.misses, dtype=np.int, copy=False).reshape(
        map.shape)
    