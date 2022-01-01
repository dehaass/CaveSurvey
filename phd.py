#!/usr/bin/env python

# Created: 2021/12/30
# Some of the weird stuff below is from this page: 
# https://stackoverflow.com/questions/22425921/pass-a-2d-numpy-array-to-c-using-ctypes


import numpy as np
from numpy.ctypeslib import ndpointer
import ctypes
import sys

#sys.path.append('/home/stuart/Documents/CaveSurvey/')

#_libPHD=ctypes.cdll.LoadLibrary('./obj/_PHD.so')
_libPHD=ctypes.cdll.LoadLibrary('/home/stuart/Documents/CaveSurvey/obj/_PHD.so')

_doublepp = ndpointer(dtype=np.uintp, ndim=1, flags='C')
_uintpp = ndpointer(dtype=np.uintp, ndim=1, flags='C')

_StationLocations = _libPHD.StationLocations
_StationLocations.argtypes = [_doublepp, ctypes.c_uint]
_StationLocations.restype = None

_ShotList = _libPHD.ShotList
_ShotList.argtypes = [_uintpp, ctypes.c_uint]
_ShotList.restype = None

def processSurvey():
    """Runs the PHD algorithm on the loaded survey"""

    if( _libPHD.processSurvey() != 0 ):
        print("Error in processSurvey")
        return False
    return True


def numStns():
    """Returns the number of stations in the survey"""
    return _libPHD.numStns()


def StationLocations():
    """Converts the C++ linked list of station locations to a numpy array"""
    
    Stns = numStns()

    # Allocate memory for the array of pointers
    # I do this from python so python knows how to free the memory
    stnList = np.zeros([Stns, 3], dtype=np.float64)
    #print(stnList)

    stnListPP = (stnList.__array_interface__['data'][0] 
        + np.arange(stnList.shape[0])*stnList.strides[0]).astype(np.uintp)

    _StationLocations(stnListPP, ctypes.c_uint(Stns))
    return stnList

def numShots():
    """Returns the number of stations in the survey"""
    return _libPHD.numShots()

def ShotList():
    """Converts the C++ linked list of shots to a numpy array"""
    
    shots = numShots()

    # Allocate memory for the array of pointers
    # I do this from python so python knows how to free the memory
    shotList = np.zeros([shots, 2], dtype=np.uintc)
    #print(shotList)

    shotListPP = (shotList.__array_interface__['data'][0] 
        + np.arange(shotList.shape[0])*shotList.strides[0]).astype(np.uintp)

    #print(shotList[2])

    _ShotList(shotListPP, ctypes.c_uint(shots))
    for i in range(len(shotList)):
        shotList[i][0] -= 1
        shotList[i][1] -= 1
    return shotList

if __name__ == '__main__':
    processSurvey()
    #print(numStns())
    stnList = StationLocations()
    #print(stnList[1])
    shotList = ShotList()
    print(shotList)