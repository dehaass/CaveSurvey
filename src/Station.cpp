#include "Station.h"


using namespace std;

// Initialize root station
Station* ROOT_STN = new Station();

// constructor
Station::Station(){
	id = up = down = left = right = x = y = z = 0;
	// Set error to '-1' to indicate it has not been set yet
	ex = ey = ez = eT = -1;
	nextStation = nullptr;
}

// Prints 'this' station. Used for debugging
void Station::print(){
	//cout << "ID= " << id << " U= " << up << " D= " << down << " L= " << left << " R= " << right << '\n';
	cout << "\nID= " << id << " x= " << x << " y= " << y << " z= " << z << "\nex= " << ex << " ey= " << ey << " ez= " << ez << " eT= " << eT << '\n';

	if(splayList.size() == 0){
		cout << "No splays\n";
	}
	else{
		cout << "Splay List:\n";
		for(int i = 0; i < splayList.size(); i++){
			splayList[i]->print();
		}
	}
}


// Prints all stations after 'this' in linked list
// Use ROOT_STN to print all stations
void Station::printStations(){
Station* currStation = this;
	cout << "Printing Stations:\n";
	while(currStation != nullptr){
		currStation->print();
		currStation = currStation->nextStation;
	}

}// printStations

// Insert the next station into the correct location
Station* Station::createNext(int ID, std::string U, std::string D, std::string L, std::string R){
	Station* prevStn;

	// if first station, populate 'this'. ROOT_STN->id is initilized to 0 before being passed.
	if( this->id == 0  ){
		this->setStation(ID, strToF(U), strToF(D), strToF(L), strToF(R));
		if( ID == 1 ) ex = ey = ez = eT = 0;
		return this;
	}

	// If this is the next sequential station, skip the other steps
	if( this->id == (ID-1) ){
		// if this is the last station in the list, or the next station is non-sequential
		if( this->nextStation == nullptr || this->nextStation->id > ID ){
			prevStn = this; // Insert after 'this'
		}else{// if the station already exists, return it 
			return this->nextStation;
		}
	}else if( ID < ROOT_STN->readID() ){ // Insert at the begining of the list

	//Create a new station and initilize it with the values
	Station* newROOT = new Station;
	newROOT->setStation(ID, strToF(U), strToF(D), strToF(L), strToF(R));
	if( ID == 1 ) newROOT->setError( 0, 0, 0 ); 
	// Insert it into the linked list
	newROOT->nextStation = ROOT_STN;
	ROOT_STN = newROOT;
	return newROOT; // Return the new station

	}else if(this->id > ID){// If the new station is non-sequential and ID is less then 'this->id'
		prevStn = ROOT_STN->find(ID); // Search starting at the ROOT_STN
		if(prevStn->id == ID) return prevStn; // if it exists, return it
	}else{
	// If the new station is non-sequential and ID is greater then 'this->id'
		prevStn = this->find(ID); // Search starting at 'this'. (Should not be before here!)
		if(prevStn->id == ID) return prevStn;// if it exists, return it
	}

	//Create a new station and initilize it with the values
	Station* newStn = new Station;
	newStn->setStation(ID, strToF(U), strToF(D), strToF(L), strToF(R));

	// Insert it into the linked list
	newStn->nextStation = prevStn->nextStation;
	prevStn->nextStation = newStn;

	return newStn; // Return the new station
}//createNext

//Returns pointer to last station before the new one or the one that aleady exists
Station* Station::find(int stnID){
	Station* currStation = this;
	Station* prevStation = nullptr;

	while(currStation != nullptr){ // Go until end of linked list
		if(currStation->id > stnID){ // Find the station AFTER where we want
			return prevStation; // Return the previous station
		}else{
			prevStation = currStation; // Iterate
			currStation = currStation->nextStation;
		}
	}// while
	return prevStation;
}// find

// initialize the station with the parameters
void Station::setStation(int ID, double U, double D, double L, double R){
	id        = ID;
	up        = U;
	down      = D;
	left      = L;
	right     = R;
	x = y = z = 0;
// Set error really high to show the station position has not been set
	ex = ey = ez = eT = -1;
	nextStation = nullptr;
}// setStation

// (re)position 'this' station based off the Path and it's errors
void Station::weightedAve( std::vector<double> Path ){

//	cout << "Before Weighted Ave:\n";
//	this->print();
//	cout << "Path:\n";
//	cout << "x= " << Path[0] << " y= " << Path[1] << " z= " << Path[2] << '\n';
//	cout << "ex= " << Path[3] << " ey= " << Path[4] << " ez= " << Path[5] << '\n';
	
	// Final error values
	double exF, eyF, ezF;

	// Weighted error:
	// ex^ = 1 / ( SUM 1/ex_i )
	exF = 1 / ( (1/ex) + (1/Path[3]) );
        eyF = 1 / ( (1/ey) + (1/Path[4]) );
        ezF = 1 / ( (1/ez) + (1/Path[5]) );

	// Weighted mean:
	// x^ = ( SUM x_i/ex_i ) / ( SUM 1/ex_i )
//finalPos = ( curPos/curErr + pathPos/pathErr ) * finalErr
	x = ( (x/ex) + (Path[0]/Path[3]) ) * exF;
	y = ( (y/ey) + (Path[1]/Path[4]) ) * eyF;
	z = ( (z/ez) + (Path[2]/Path[5]) ) * ezF;

	this->setError( exF, eyF, ezF );
	//cout << "After Weighted Ave:\n";
	//this->print();

}// weightedAve

void Station::resetStns(){

	Station* currStn = this->nextStation;
	while( currStn != nullptr ){
		currStn->setPos(0,0,0);
		currStn->setError(-1,-1,-1);
		currStn = currStn->nextStation;
	}
}// resetStns

// Find the max and min of all cartesian coordinates
void Station::maxPos(double *minX, double *minY, double *minZ, double *maxX, double *maxY, double *maxZ){

	Station* currStation = ROOT_STN;
	double x, y, z;
	// Initialize all values to 0
	*minX = *minY = *minZ = *maxX = *maxY = *maxZ = 0;

	while( currStation != nullptr ){ // Go until the end of the list

		currStation->readPos(&x, &y, &z); 

		if( x > (*maxX) ){ (*maxX) = x;
		}else if( x < (*minX) ) (*minX) = x;

		if(y > *maxY ){ *maxY = y;
		}else if(y < *minY ) *minY = y;

		if(z > *maxZ ){ *maxZ = z;
		}else if(z < *minZ ) *minZ = z;

		currStation = currStation->nextStation;

	}// while
}// maxPos

// Count number of stations in linked list
unsigned int Station::numStn(){

	Station* currStation = this;
	unsigned int num = 0;

	while( currStation != nullptr ){
		num++;
		currStation = currStation->nextStation;	
	}//while
	return num;
}// numStn

//Read the stations relative position
void Station::readPos(double* X, double* Y, double* Z){
    *X = x;
    *Y = y;
    *Z = z;
}// readPos

//Read the stations relative error
void Station::readError(double* X, double* Y, double* Z){
    *X = ex;
    *Y = ey;
    *Z = ez;
}// readError

// Find the total error of the station
double Station::totalError(){
	return eT;
}// totalError

// Set the stations position
void Station::setPos(double X, double Y, double Z){
   x = X;
   y = Y;
   z = Z;
}// setPos

// Set the stations Errors
void Station::setError(double X, double Y, double Z){
   ex = X;
   ey = Y;
   ez = Z;
   
   ex==(-1) ? eT=(-1) : eT = sqrt( pow(ex, 2) + pow(ey, 2) + pow(ez, 2) );
}// setError

// Return the station ID
int Station::readID(){
	return id;
}// readID

double Station::getX(){
	return x;
}// getX

double Station::getY(){
	return y;
}// getY

double Station::getZ(){
	return z;
}// getZ

void Station::readUDLR(double* U, double* D, double* L, double* R){
	*U = up;
	*D = down;
	*L = left;
	*R = right;
}// readUDLR

void Station::pushSplay(Splay* splay){
	splayList.push_back(splay);
}

// Return a pointer to the splays vector
std::vector<Splay*>* Station::getSplayList(){
	cout << "Splay List:\n";
	for(unsigned int i = 0; i < splayList.size(); i++){
		cout << "Splay " << i << ":\n";
		splayList[i]->print();
	}
	return &splayList;
}// getSplayList