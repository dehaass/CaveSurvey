#ifndef SURVEYPLOT_H
#define SURVEYPLOT_H

#include "Shot.h"
#include "Station.h"
#include "dataFun.h"
#include "userInterface.h"
#include "svgMap.h"


const double scale = 25;

// Plot the cave .svg
void plotCave(std::ofstream&);

int createMap();

#endif // SURVEYPLOT_H
