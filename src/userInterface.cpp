#include "userInterface.h"

// Main Menu
int menu(){
	std::string in;

	// Title 
	cout << "Plot Her Deeply (PHD), a cave survey tool V1.0A\n";
	cout << "Welcome to Stuart's 'PHD' Tool (working title)\n\n";

	while(1){
		cout << "Please input 'f' to plot from (f)ile, 'm' to (m)anually input, 'g' to (g)ive'er, or 'e' to (e)xit:\n";
		cin >> in;
		switch( in.at(0) ){
			case 'f':
			case 'F':
				return ( fromFile() );
			case 'm':
			case 'M':
				return (  fromSI() );
			case 'e':
			case 'E':
				return -1;
			case 'g':
			case 'G': 
				return( giver() );
			case 's':
			case 'S':
				return( processSurvey() );
			default:
				cout << "Invalid input, dummy.\n";
				break;
		}//switch
	}// while

return 0;
}// menu

// Debugging program. Bypass user input
extern "C"{
    int giver(){

        std::ifstream surveyFile;
        std::ofstream cavePlot;

        std::string survey = "./aux/RKVSC_Survey_Notes.csv";

        surveyFile.open(survey);
        // Check for sucessful file opening
        if( !surveyFile.good() ){
            cout << "surveyFile is not good";
            return 1;
        }
        populateStations(surveyFile);
        surveyFile.close();

        surveyFile.open(survey);
        populateShots(surveyFile);
        surveyFile.close();

        // Populate the station relative positions
        //ROOT_STN->printStations();
        //ROOT_SHOT->printShots();
        ROOT_SHOT->populatePos();
        //ROOT_STN->printStations();

        // populate the map data
        caveMap = new svgMap;
        caveMap->mapName = "Radio Killed the Video Survey Cave";

        cavePlot.open( "RKVSC.svg", std::ofstream::out | std::ofstream::trunc);
        if( !cavePlot.good() ){
            cout << "cavePlot is not good!";
            return 1;
        }
        plotCave(cavePlot);
        cavePlot.close();

        return 0;

    }// giver
}

extern "C"{
    int processSurvey(){

        std::ifstream surveyFile;
        //std::ofstream cavePlot;

        std::string survey = "./aux/RKVSC_Survey_Notes.csv";

        surveyFile.open(survey);
        // Check for sucessful file opening
        if( !surveyFile.good() ){
            cout << "surveyFile is not good";
            return 1;
        }
        populateStations(surveyFile);
        surveyFile.close();

        surveyFile.open(survey);
        populateShots(surveyFile);
        surveyFile.close();

        // Populate the station relative positions
        ROOT_SHOT->populatePos();
        //ROOT_STN->printStations();
        //ROOT_SHOT->printShots();
        //ROOT_STN->printStations();

        // populate the map data

        return 0;

    }// stnloc
}
// Read survey from '*.csv' file
int fromFile(){

        std::ifstream surveyFile;
	std::string survey;

	cout << "Please provide the name of the cave survey file to be plotted or (e)xit\n";
	cin >> survey;
	if( survey == "e" ) return -1; // if (e)xit
	surveyFile.open(survey);
	while( !surveyFile.good() ){ // Check for successful opening
		cout << "Error reading file.\n";
	        cout << "Please provide the name of a survey file or (e)xit";
		cin >> survey;
		if( survey == "e" ) return -1; // if (e)xit
	}// while

	populateStations(surveyFile);
	surveyFile.close();

	surveyFile.open(survey);
	populateShots(surveyFile);
	surveyFile.close();

	return 0;
}// fromFile

int fromSI(){

	std::string up, down, left, right, dist, bear, clino;
	std::string fromID, toID;
	int rval;
	ROOT_STN  = new Station();
	ROOT_SHOT = new Shot();
	Station* currStn  = ROOT_STN;
	Shot*    currShot = ROOT_SHOT;

	cout << "Welcome to the cave survey input tool!\n\n";
	cout << "Please input your first Station or (e)xit\n";

	cout << "\nFrom Station:\n";
	cout << "Stn. ID: 1\n";
	if( readNumSI("Up: ", &up)          != 0 ) return -1;
	if( readNumSI("Down: ", &down)      != 0 ) return -1;
	if( readNumSI("Left: ", &left)      != 0 ) return -1;
	if( readNumSI("Right: ", &right)    != 0 ) return -1;
	currStn = currStn->createNext( 1, up, down, left, right );
	cout << '\n';
	currStn->print();
	cout << '\n';
	toID = std::to_string (1);

	while(1){

		rval = readNumSI("Input distance to sequential shot, (e)xit or (n)ew 'from' Station: \n", &dist);
		if( rval == -1 ){ // if (e)xit
		       	return 0;
		}else if( rval == 1 ){ // if (n)ew station
			cout << "\nFrom Station:\n";
			if( readNumSI("Stn. ID or (e)xit: ", &fromID) != 0 ) return 0;
			currStn = ROOT_STN->find( stoi(fromID) );
			if( currStn->readID() != stoi(fromID) ){
				if( readNumSI("Up: ", &up)          != 0 ) return -1;
				if( readNumSI("Down: ", &down)      != 0 ) return -1;
				if( readNumSI("Left: ", &left)      != 0 ) return -1;
				if( readNumSI("Right: ", &right)    != 0 ) return -1;
				currStn = currStn->createNext( stoi(fromID), up, down, left, right );
			}
			currStn->print();
			cout << "\nShot info:\n";
			if( readNumSI("Distance: ", &dist)     != 0 ) return -1;
		}else fromID = toID;// Copy 'toID' into 'fromID'

		if( readNumSI("Bearing: ", &bear)      != 0 ) return -1;
		if( readNumSI("Inclination: ", &clino) != 0 ) return -1;
		cout << "\nFrom Station: " << currStn->readID() << '\n';
		cout << "Distance: " << dist << " Bearing: " << bear << " Inclination: " << clino << "\n";

		cout << "\nTo Station:\n";
		if( readNumSI("Stn. ID: ", &toID) != 0 ) return -1;
		currStn = ROOT_STN->find( stoi(toID) );
		if( currStn->readID() != stoi(toID) ){
			if( readNumSI("Up: ", &up)          != 0 ) return -1;
			if( readNumSI("Down: ", &down)      != 0 ) return -1;
			if( readNumSI("Left: ", &left)      != 0 ) return -1;
			if( readNumSI("Right: ", &right)    != 0 ) return -1;
		cout << fromID << " " << toID;
			currStn = currStn->createNext( stoi(toID), up, down, left, right );
		}
		cout << '\n';
		currStn->print();
		cout << '\n';
		currShot = currShot->createNext( stoi(fromID), stoi(toID), stof(dist), stof(bear), stof(clino) );


	}// while

	return 0;
}// fromSI

// Print message and return viable user input
int readNumSI(std::string message, std::string* str){

	//Clean the IO
	getline(cin, *str);

	//until they get it right
	while(1){
		cout << message;
		cin >> *str;

		if( str->empty() ){ // empty string
			cout << "Please enter a number\n";
		}else if( *str == "e" ){ // (e)xit
		       	return -1;
		}else if( *str == "n" ){ // (n)ew station
			return 1;
		// if not a number
		}else if( str->find_first_not_of("0123456789.-") != std::string::npos ){
			cout << "Invalid input\n";
		}else return 0;

	}//while
	
	//Should never reach this point
	return -1;
}// readNumSI
