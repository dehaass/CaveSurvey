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

// Goes through the stations and populates the splays from the UDLR data
void populateSplaysFromUDLR(){
	Station* currStn = ROOT_STN;
	Shot* currShot = ROOT_SHOT;
	Shot* tempFromShot = nullptr;
	Shot* tempToShot = nullptr;

	std::vector<Splay> splays;

// Go through every station and find the first from and to shots that are connected to it.
	while(currStn != nullptr){
		while(currShot != nullptr){
			if(currShot->readFromStn() == currStn && tempFromShot == nullptr){
				tempFromShot = currShot;
				//break;
			}else if(currShot->readToStn() == currStn && tempToShot == nullptr){
				tempToShot = currShot;
			}
			if(tempFromShot != nullptr && tempToShot != nullptr){
				break;
			}
			currShot = currShot->nextShot;
		}

		if(tempFromShot != nullptr || tempToShot != nullptr){
			splaysFromUDLR(splays, currStn, tempFromShot, tempToShot);
		}
		//currStn->print();
		tempFromShot = nullptr;
		tempToShot = nullptr;

		currStn = currStn->nextStation;
		currShot = ROOT_SHOT;
	}

}

// Uses a station and a from or to shot to convert UDLR data into splay shots
void splaysFromUDLR(std::vector<Splay>& splays, Station *stn, Shot *fromShot, Shot *toShot){
// Left and right station data has to be oriented perpendicular to the cave passage
// Conventionally, left and right distances are taken while facing forward to the next station
// Up and down distances are conventionally pure z-axis distances so don't need to be oriented
/*          |
 *    -x +y | +x +y
 *     -----|-----
 *    -x -y | +x -y
 *          |
 */

	double u, d, l, r;
	double xs, ys, zs = 0; // Shot deltas

// Default direction is forward (from shot to to shot)
// Some stations don't have from shots so we use the to shot's direction
// For instance, the last shot on a branch won't have any from shots
	if(fromShot == nullptr){
		toShot->readDeltas(&xs, &ys, &zs);
		xs *= -1;
		ys *= -1;
	}else{
		fromShot->readDeltas(&xs, &ys, &zs);
	}

	stn->readUDLR(&u, &d, &l, &r);

// Push the up splay
	stn->pushSplay(new Splay(0.0, 0.0, u));
// Push the down splay
	stn->pushSplay(new Splay(0.0, 0.0, (-1.0)*d));

	// Normalize x,y vector of shot
	double Shotmag = sqrt(xs*xs + ys*ys);

// When rotating vectors by 90 degrees, the x and y components are swapped.
// (x,y) = (-y,x) is a +90 degree rotation
// (x,y) = (y,-x) is a -90 degree rotation

// calculate left splay
// rotate by +90 and scale by left
	//x = l*(-1.0)*ys/Shotmag;
	//y = l*xs/Shotmag;
	//stn->pushSplay(&Splay(x, y, 0));
	//stn->pushSplay(&Splay(l*(-1.0)*ys/Shotmag, l*xs/Shotmag, 0));
	stn->pushSplay(new Splay(l*(-1.0)*ys/Shotmag, l*xs/Shotmag, 0));

// calculate right splay
// rotate by -90 and scale by right
	//stn->pushSplay(&Splay(l*ys/Shotmag, l*(-1.0)*xs/Shotmag, 0));
	stn->pushSplay(new Splay(l*ys/Shotmag, l*(-1.0)*xs/Shotmag, 0));

}