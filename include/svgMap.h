#ifndef SVGMAP_H
#define SVGMAP_H

#include <cmath>
#include "Shot.h"
#include "Station.h"
#include "dataFun.h"
#include "userInterface.h"
#include "surveyPlot.h"

class svgMap{

    public:
        svgMap();
	std::string mapName;
	std::string fileHeader();
	std::string fileFooter();
	std::string XYMap(Shot*, std::string colour);
	std::string XZMap(Shot*, std::string colour);
	std::string YZMap(Shot*, std::string colour);
	std::string XYstnMarker(Station* stn);
	std::string XZstnMarker(Station* stn);
	std::string YZstnMarker(Station* stn);
	std::string metrics();
	std::string background();
        std::string title();
	std::string watermark();
	
    private:
	double mapWidth, mapHeight, mapDepth;
	double xoffset, yoffset, zoffset;
	double pageWidth, pageHeight, pageDepth;
	double horDist, vertDist;
	double metricHeight, titleHeight, tinyFont, smallFont, largeFont, pathWidth;
	unsigned int numStn, numShot;
	std::string textBlock(double x, double y, int size, std::string pos, std::string str);
};

extern svgMap* caveMap;

#endif // SVGMAP_H
