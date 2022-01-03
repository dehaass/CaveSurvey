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
#include "Splay.h"


extern "C"{
    void StationLocations(double **stnList, const unsigned int numStn);
}

extern "C"{
    unsigned int numStns();
}

extern "C"{
   void splayEdgesGenerator(double **verts, const unsigned int sizeofVerts, unsigned int **edges, const unsigned sizeofEdges);
}

#endif // BLENDEROUTPUTS_H