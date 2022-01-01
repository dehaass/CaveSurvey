#ifndef SHOT_H
#define SHOT_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <array>
#include <vector>
#include "Station.h"
#include "dataFun.h"

// Need to make sure that 'Shot' knows that 'Station' exists
class Station;

class Shot{

	// They're friends!!
	friend class Station;
    friend class svgMap;

	protected:
		// Pointers to the stations involved in this shot
		Station *fromStn, *toStn;

		// These are the relative cartesian coordinates and errors of fromStn -> toStn 
		double dx, dy, dz;
		double ex, ey, ez;
	public:
		// Public pointer used in shot linked list
		Shot* nextShot;

		//Constructor
		Shot();

		// Print all shots after 'this' shot (Use ROOT_SHOT to print everything)
		void printShots();

		// Prints 'this' shot. Used for debugging
		void print();

		// Inputs the new shot in the correct order and returns a pointer to it
		Shot* createNext( int, int, double, double, double );

		// Returns the location to insert a new shot with this 'fromID' and 'toID'
		Shot* find(int, int);

		// Finds the next node during a Depth-First Search of the station tree
		std::vector<Shot*> findBranches(bool reverse);

		// Calculates relative cartesian coordinates from dist, bear, and clino
		// Inputs all the data for the shot
		void setShot(Station*, Station*, double, double, double);

		void findPaths( std::vector<double> Path, Station* rootStn, bool reverse );

		// Populates the station position using loop closures by following all paths to each station
		void populatePos();

		// Populates station positions using single shot for fast, but less accurate, station positioning
		void basicPos();

		//Returns the total error of the shot
		double totalError();

		// Calculates interesting metrics about the survey
		void metrics(double *hor, double *vert, unsigned int *numShot);

		// Returns the 'fromStn'
		Station* readFromStn();

		// Returns the 'toStn'
		Station* readToStn();	

		// Returns the 'fromStn' ID
		unsigned int readFromStnID();

		// Returns the 'toStn' ID
		unsigned int readToStnID();

		// Reads relative cartesian coordinates of shot 
		void readDeltas(double*, double*, double*);

		// reads the error values
		void readError(double* X, double* Y, double* Z);

		// Reads position of the 'fromStn'
		void fromPos(double* X, double* Y, double* Z);

		void debugOut( std::string str );

};//Shot

// Creates global variable of pointer to the root shot
extern Shot* ROOT_SHOT;

#endif // SHOT_H
