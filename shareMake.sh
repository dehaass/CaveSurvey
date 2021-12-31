#!/bin/bash

g++ -shared -o _PHD.so ./obj/src/*.o

mv _PHD.so ./obj/
