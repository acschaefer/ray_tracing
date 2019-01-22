#!/usr/bin/env python

import numpy as np
import build.ray_tracing_python as rtp


def trace1d(start, end, map):
    map1 = rtp.gridmap1(map.shape, map.size)
    rtp.trace1d(start, end, map1)
    map.hits = map1.hits
    map.misses = map1.misses


def trace2d(start, end, map):
    map2 = rtp.gridmap2(map.shape, map.size)
    rtp.trace2d(start, end, map2)
    map.hits = map2.hits
    map.misses = map2.misses


def trace3d(start, end, map):
    map3 = rtp.gridmap3(map.shape, map.size)
    rtp.trace3d(start, end, map3)
    map.hits = map3.hits
    map.misses = map3.misses
    