#include "surveyPlot.h"

int createMap(){

	ROOT_SHOT->basicPos();
	//ROOT_SHOT->populatePos();
	caveMap = new svgMap;
	std::ofstream cavePlot;

	cout << "Please enter the title of the survey:\n";
	//Clean IO
	getline(cin, caveMap->mapName);
	// Get name
	getline(cin, caveMap->mapName);



	cavePlot.open( caveMap->mapName + ".svg", std::ofstream::out | std::ofstream::trunc);
	if( !cavePlot.good() ){
		cout << "cavePlot is not good!";
		return 1;
	}
	plotCave(cavePlot);
	cavePlot.close();

	return 0;
}// createMap

void plotCave(std::ofstream& cavePlot){

	cavePlot << caveMap->fileHeader();
	cavePlot << caveMap->background();

/*	
	cavePlot << caveMap->XYMap(ROOT_SHOT, "lightgrey");
	cavePlot << caveMap->XZMap(ROOT_SHOT, "lightgrey");
	ROOT_STN->resetStns();
	ROOT_SHOT->basicPos();
*/	

	cavePlot << caveMap->XYMap(ROOT_SHOT, "black");
	cavePlot << caveMap->XZMap(ROOT_SHOT, "black");

	//cavePlot << caveMap->XYstnMarker(ROOT_STN);
	//cavePlot << caveMap->XZstnMarker(ROOT_STN);

	cavePlot << caveMap->title();
	cavePlot << caveMap->watermark();
	cavePlot << caveMap->metrics();

	cavePlot << caveMap->fileFooter();

}// plotCave

