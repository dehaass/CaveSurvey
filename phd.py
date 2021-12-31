#!/usr/bin/env python

import ctypes

_libPHD=ctypes.cdll.LoadLibrary('./obj/_PHD.so')

_libPHD.giver()

#def ctypes_giver():

 #   return _libfib.fib(ctypes.c_int(a))
