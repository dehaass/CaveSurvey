/******Plot Her Deeply - A Cave Survey Tool******
 ******A command line tool for producing line plots from cave surveys.
 ******Created: July 26, 2015
 ******Created by: Stuart de Haas
 */

#include "Shot.h"
#include "Station.h"
#include "dataFun.h"
#include "userInterface.h"
#include "surveyPlot.h"
#include "svgMap.h"


using std::cout;

#define DEBUG 0

// Not used (yet)
#ifdef DEBUG
#define DEBUG_PRINT(x) cout << x << endl;
#else
#define DEBUG_PRINT(x)
#endif

int main(){

	processSurvey();

	// Run the menu program and save its return value
	int menuStatus = menu();

	// If the menu program exited normally, produce the line plot
	if( menuStatus == 0){
		ROOT_STN->printStations();
		ROOT_SHOT->printShots();
		
		createMap();

	// If the menu exits with a negative value, the user force exited the program
	}else if( menuStatus < 0 ){
		cout << "User exited";
	
	// If the menu exits with a positive value, there was a program error
	}else{
	
		cout << "An error occured";
	}

	return 0;
} // main

