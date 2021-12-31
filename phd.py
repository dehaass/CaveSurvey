#!/usr/bin/env python

import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes

_doublepp = ndpointer(dtype=np.uintp, ndim=1, flags='C')


_libPHD=ctypes.cdll.LoadLibrary('./obj/_PHD.so')

_StationLocations = _libPHD.StationLocations
_StationLocations.argtypes = [_doublepp, ctypes.c_uint]
_StationLocations.restype = None
#_libPHD.StationLocations.argtypes = [ctypes.POINTER(ctypes.c_double)]
#_libPHD.giver()

def ctypes_giver():
    return _libPHD.giver()

def ctypes_numStns():
    return _libPHD.numStns()

def StationLocations():
    numStns = ctypes_numStns()

    stnList = np.zeros([numStns, 3], dtype=np.float64)
    #print(stnList)

    stnListPP = (stnList.__array_interface__['data'][0] 
        + np.arange(stnList.shape[0])*stnList.strides[0]).astype(np.uintp)

    #stnList = (ctypes.c_double * numStns)()
    _StationLocations(stnListPP, ctypes.c_uint(numStns))
    return stnList

ctypes_giver()
print(ctypes_numStns())
print(StationLocations())
