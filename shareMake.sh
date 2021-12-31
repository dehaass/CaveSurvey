#!/bin/bash

g++ -shared -o _PHD.so ./obj/Release/src/*.o

mv _PHD.so ./obj/
