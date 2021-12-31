#ifndef BLENDEROUTPUTS_H
#define BLENDEROUTPUTS_H


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Shot.h"
#include "Station.h"
#include "dataFun.h"


extern "C"{
    void StationLocations(double **stnList, const unsigned int numStn);
}

extern "C"{
    unsigned int numStns();
}

#endif // BLENDEROUTPUTS_H