#include "Shot.h"

using namespace std;

// PI is needed for trig calculations
// Why is it not defined in cmath??
const double  PI    = 3.1415926535897932384;
const double eDist  = 0.01; // [m] distance error
const double eBear  = 1.0 *(PI/180.0); // [Rad] Bearing error
const double eClino = 1.0 *(PI/180.0); // [Rad] Clinometer error
const int MAX_DEPTH = 10;

// Initializes root shot pointer to a new shot
Shot* ROOT_SHOT = new Shot();

//Constructor. Initializes everything to 0/null
Shot::Shot(){
dx = dy = dz = 0;
fromStn = toStn = nullptr;
}

// Print all shots after 'this' shot (Use ROOT_SHOT to print everything)
void Shot::printShots(){
 Shot* currShot = this;
	while(currShot != nullptr){
		currShot->print();
		currShot = currShot->nextShot;
	}
}//printShots

// Prints 'this' shot. Used for debugging
void Shot::print(){
	cout << "from " << fromStn->id << " to " << toStn->id << " dx= " << dx << ", dy= " << dy << ", dz= " << dz << endl;
}// print

// Inserts a new shot in correct position in linked list
Shot* Shot::createNext( int fromID, int toID, double dist, double bear, double clino ){

	// If this shot is a 'backshot', reverse it
	if(fromID > toID){
		int ID = toID;
		toID = fromID;
		fromID = ID;
		bear += 180;
		if( bear >= 360 ) bear = bear - 360;
		clino *= (-1);
	}

	// Use the ID numbers to find the associated station pointer
	Station *from;
        Station	*to;
	from = ROOT_STN->find(fromID);
	to = from->find(toID);
	if (to == nullptr ) to = ROOT_STN->find(toID); // If they are out of order this will still find it

	// if this is the ROOT_SHOT
	if( this->fromStn == nullptr ){// An un initialized shot will have all nullptrs
		this->setShot(from, to, dist, bear, clino);
		return this;
	}

	// If it's not the  root, create a new shot to be put into the list
	Shot* newshot = new Shot;
	newshot->setShot(from, to, dist, bear, clino);

	// Figure out where to insert this new shot and then do it
	Shot* prevShot = ROOT_SHOT->find(fromID, toID);
	newshot->nextShot = prevShot->nextShot;
	prevShot->nextShot = newshot;

	return newshot;
}// createNext

// Returns the location to insert a new shot with this 'fromID' and 'toID'
Shot* Shot::find(int fromID, int toID){
	Shot* currShot = this;
	Shot* prevShot = nullptr;

	// While we are not at the end of the linked list
	while(currShot != nullptr){

		// If the 'fromID' matches, it means that station has multiple shots.
		// start looking for the 'toID'
		if(currShot->fromStn->id == fromID ){
			while(currShot != nullptr){ // While not at the end

				// Go until either we pass the target 'toID' location, or the 'fromID' changes
				if(currShot->toStn->id > toID || currShot->fromStn->id != fromID ){
					return prevShot;
				}else{
					//increment to check the next shot
					prevShot = currShot;
					currShot = currShot->nextShot;
				}
			}// while
			return prevShot;

		// If we pass it without finding a match, this station does not have any 'from' shots yet.
		}else if(currShot->fromStn->id > fromID){
				return prevShot;
	        }
			prevShot = currShot; // Increment
			currShot = currShot->nextShot;

	}// while

	// Made it to the end of the linked list. Return last shot
	return prevShot;
}// findŝ

// Finds the next node during a Depth-First Search of the station tree
std::vector<Shot*> Shot::findBranches(bool reverse){

	Shot* currShot = ROOT_SHOT;
	Station* node;
	std::vector<Shot*> branches;

	if( reverse == TRUE ){
		node = this->fromStn;
	}else{
		node = this->toStn;
	}// if reverse

	while( currShot != nullptr ){
		if( (currShot->toStn == node || currShot->fromStn == node) && currShot != this){
			branches.push_back(currShot);
		}
		currShot = currShot->nextShot;
	}// while

	return branches;
}// findBranches

// Calculates relative cartesian coordinates from dist, bear, and clino
// Inputs all the data for the shot
void Shot::setShot(Station* from, Station* to, double dist, double bear, double clino){
/*          |
 *    -x +y | +x +y
 *     -----|-----
 *    -x -y | +x -y
 *          |
 */

	// If going 'right' +x
	if(bear > 180){
		dx = (-1); //Negative direction
		bear = 360 - bear; // move bearing to left two quadrants
	}else dx= 1; // Positive direction (right) x

	//If going 'backwards' -y
	if(bear > 90){
		dy = -1; // negative direction
		bear = 180 - bear; // move to top right quadrant
	}else dy=1;// positive direction (forwards) +y

	// If going 'down' -z
	if(clino < 0){
		dz = -1; // negative direction
		clino *= -1; // change to first quadrant
	}else dz=1; // Positive direction (up) +z

	bear  *= (PI/180); // Convert to Rad
	clino *= (PI/180); // Convert to Rad

	// dx (+-) ex = (dist(+-)eDist) * cos(clino (+-) eClino) * sin(bear (+-) eBear)
	// sin(a (+-) b) = sin(a)cos(b) (+-) sin(b)cos(a)
	// cos...
	// Total_error = 1 / SUM( 1 / error_i )
	dx *= dist * (cos(clino) * cos(eClino)) * (sin(bear) * cos(eBear));
	dy *= dist * (cos(clino) * cos(eClino)) * (cos(bear) * cos(eBear));
	dz *= dist * (sin(clino) * cos(eClino));

	// If the displacement is zero, will give 'Nan'
	// Set errors to default '0' then evaluate true value if required
	ex = ey = ez = 0;
	if(dx != 0)
	ex = abs(dx) * sqrt( pow(eDist/dist,2)
			+ pow( ((sin(clino)*sin(eClino))/(cos(clino)*cos(eClino))), 2)
			+ pow( ((sin(eBear)*cos(bear))/(sin(bear)*cos(eBear))),
			 2) );
	if(dy != 0)
	ey = abs(dy) * sqrt( pow(eDist/dist,2)
			+ pow( ((sin(clino)*sin(eClino))/(cos(clino)*cos(eClino))), 2)
			+ pow( ((sin(eBear)*sin(bear))/(cos(bear)*cos(eBear))), 2) );
	if(dz != 0)
	ez = abs(dz) * sqrt( pow(eDist/dist,2)
			+ pow( ((sin(eClino)*cos(clino))/(sin(clino)*cos(eClino))), 2) );

	fromStn = from;
	toStn   = to;
	nextShot = nullptr;
}// setShot

// Populates the station position using loop closures by following all paths to each station
void Shot::populatePos(){

	Shot *currShot  = this;
	// The first shot will be forwards
	bool reverse = FALSE;
	// Path contains the current path data used in recursive 'findPaths()'
	// Path == x, y, z, ex, ey, ez, nodeDepth
	std::vector<double> Path (7);

	while( currShot != nullptr ){
		// If uninitialized station
		if( currShot->toStn->totalError() == -1 ){
			debugOut( "---------- "
				+ std::to_string(currShot->toStn->readID())
				+ " --------------");
			Path.assign(7,0);
			currShot->findPaths( Path, currShot->toStn, reverse );
		}
		currShot = currShot->nextShot;
	}//while
}// populatePos

// Populates station positions using single shot for fast, but less accurate, station positioning
void Shot::basicPos(){

	Shot* currShot = this;
	double x, y, z, dX, dY, dZ;

	while( currShot != nullptr ){
		currShot->fromStn->readPos( &x, &y, &z );
		currShot->readDeltas( &dX, &dY, &dZ );
		currShot->toStn->setPos( x+dX, y+dY, z+dZ );
		currShot->fromStn->readError( &x, &y, &z );
		currShot->readError( &dX, &dY, &dZ );
		currShot->toStn->setError( x+dX, y+dY, z+dZ );

		currShot = currShot->nextShot;
	}// while
}// basicPos

// Function recursively called to perform a depth first search of possible paths to the current root station
void Shot::findPaths( std::vector<double> Path, Station* rootStn, bool reverse ){

	Station* currStn;
	unsigned int i;

	// if reverse is true, curr=from. else curr=to
	reverse == TRUE ? currStn = this->fromStn : currStn = this->toStn;

	// If this is the root shot of the Path, don't increase path length and such
	if( rootStn->totalError() == -1 && currStn == rootStn ){
		Station* fromStn;
		reverse == TRUE ? fromStn = this->toStn : fromStn = this->fromStn;
		double x, y, z;

		fromStn->readPos( &x, &y, &z );

		reverse == TRUE ? rootStn->setPos( x-dx, y-dy, z-dz )
			: rootStn->setPos( x+dx, y+dy, z+dz );

			fromStn->readError( &x, &y, &z );
			rootStn->setError( x+ex, y+ey, z+ez );
	}else{

		//Increment the nodeDepth
		Path[6]++;

		// Reasons to giveup
		if( Path[6] >= MAX_DEPTH ){
			debugOut("MAX_DEPTH");
		       	return;
		}


		// If the Path formed a loop back to rootStn
		if ( currStn == rootStn ){
			debugOut("loop");
		       	return;
		}

		// Only set Path if all conditions are met
		if( reverse == TRUE ){
			Path[0] -= dx; // Set Path displacements
			Path[1] -= dy;
			Path[2] -= dz;
		}else{
			Path[0] += dx; // Set Path displacements
			Path[1] += dy;
			Path[2] += dz;
		}// if reverse

		// Increase Path error values
		Path[3] += ex;
		Path[4] += ey;
		Path[5] += ez;

		// if 'currStn' has been set, this is the end of a successful 'Path'
		if( currStn->totalError() != -1 ){
			// Node position/error variables
			double nx, ny, nz;
			debugOut("Successful Path");
			currStn->readPos( &nx, &ny, &nz );
			Path[0] = nx - Path[0]; // Set Path displacements
			Path[1] = ny - Path[1];
			Path[2] = nz - Path[2];
			// Increase Path error values
			currStn->readError( &nx, &ny, &nz );
			Path[3] += nx;
			Path[4] += ny;
			Path[5] += nz;
			// Do a weighted average of the 'Path' and the current position
			rootStn->weightedAve( Path );
			return;
		}
	}// if not path root shot

	// if this is just an ordinary node, we must recursively go deeper into the tree
	// Find all the branches associated with the current node (stn)
	std::vector<Shot*> branches = this->findBranches(reverse);

	for( i=0; i<branches.size(); i++ ){
		reverse = (branches[i]->toStn == currStn) ? TRUE : FALSE;

		debugOut( "From: "
			+ std::to_string( reverse == TRUE ? branches[i]->toStn->readID() : branches[i]->fromStn->readID() )
		        + " To: "
			+ std::to_string( reverse == FALSE ? branches[i]->toStn->readID() : branches[i]->fromStn->readID() ) );
		branches[i]->findPaths(Path, rootStn, reverse);
	}// for all branches off node

	// Will return once all nodes below here have been explored
	//debugOut("Branch Explored");
	return;
}// findPaths

// Finds the total error for the shot
double Shot::totalError(){
	return sqrt( pow(ex, 2) + pow(ey, 2) + pow(ez, 2) );
}// totalError

// Calculates interesting metrics about the survey
// Should be called from ROOT_SHOT
void Shot::metrics(double *hor, double *vert, unsigned int *numShot){

	Shot* currShot = this;
	double x, y, z;
	// Initialize all values to 0
	*hor = *vert = 0;
	*numShot = 0;

	// Find number of Stations
	while( currShot != nullptr ){ // Go until the end of the list
		//Increment the number of shots
		(*numShot)++;

		// Read the relative displacement of each shot
		currShot->readDeltas(&x, &y, &z);
		*hor += sqrt( pow(x,2) + pow(y,2) ); // Horizontal distance is sqrt(x^2+y^2)
		*vert += abs(z); // Vertical distance only depends on z-axis

		currShot = currShot->nextShot; // Increment
	}// while
}// metrics

// Returns the 'fromStn'
Station* Shot::readFromStn(){
return fromStn;
}// readFrom

// Returns the 'toStn'
Station* Shot::readToStn(){
return toStn;
} // readTo

// Reads relative cartesian coordinates of shot
void Shot::readDeltas(double* X, double* Y, double* Z){
	*X = dx;
	*Y = dy;
	*Z = dz;
}// readDeltas

// Reads error values
void Shot::readError(double* X, double* Y, double* Z){
	*X = ex;
	*Y = ey;
	*Z = ez;
}// readError

// Reads position of the 'fromStn'
void Shot::fromPos(double* X, double* Y, double* Z){
	fromStn->readPos(X, Y, Z);
}// fromPos

void Shot::debugOut( std::string str ){

	cout << '\n' << str << '\n';

}// debugOut
