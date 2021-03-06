#include "dataFun.h"

// Convert a string to double with error checking
double strToF(std::string str){
	if(str.empty()) return 0;
	return stof(str);
}

// Convert a string to int with error checking
int strToL(std::string str){
	if(str.empty()){
		cout << "'id' is 'NULL' \n";
		return 1;
	}
	return stoi(str);
}

// Read through file and input stations in order
int populateStations(std::ifstream& surveyFile){

	std::string dumb, id, u, d, l, r; 
	// Initilize root station
	Station* currStn = ROOT_STN;


	// getline( fileStream, string, substring)
	// 'getline' will read through the fileStream until it encounters a substring and save it to a string.
	// 'dumb' is used to remove excess characters and newlines
	do{
		getline(surveyFile, id, ',');
		if( id.empty() ) break; // if id is empty, something is wrong
		getline(surveyFile, u, ',');
		getline(surveyFile, d, ',');
		getline(surveyFile, l, ',');
		getline(surveyFile, r, '\n');
		currStn = currStn->createNext(strToL(id), u, d, l, r);
	}while( getline(surveyFile, dumb, '\n') );

	return 0;
}// populateStations

// Read through file and input shots in order
int populateShots(std::ifstream& surveyFile){

	std::string dumb, fromID, toID, dist, bear, clino;
	// Initillize root shot
	Shot* currShot = ROOT_SHOT;

	// read the first character. Should be the 'fromID'
	getline(surveyFile, fromID, ',');

	// getline( fileStream, string, substring)
	// 'getline' will read through the fileStream until it encounters a substring and save it to a string.
	// 'dumb' is used to remove excess characters and newlines
	while( getline(surveyFile, dumb, '\n') ){
		getline(surveyFile, dumb, ',');
		getline(surveyFile, dist, ',');
		getline(surveyFile, bear, ',');
		getline(surveyFile, clino, ',');
		getline(surveyFile, dumb, '\n');
		getline(surveyFile, toID, ',');
		if( fromID == toID ) break; // Don't know why this happens. yet. But it means we are done.
		if( !dist.empty() ){ // if there is no 'dist' it means this is a placeholder shot. Skip it.
			currShot = currShot->createNext( strToL(fromID), strToL(toID), strToF(dist), strToF(bear), strToF(clino) );
		}
		// toID becomes the next fromID for the next shot
		fromID = toID;
	}// while
	return 0;
}// populateShots



