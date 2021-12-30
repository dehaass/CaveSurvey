#ifndef STATION_H
#define STATION_H


#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>
#include "Shot.h"
#include "dataFun.h"

class Station{
	
	// They're friends!!
    friend class Shot;
    friend class svgMap;

    protected:
    		// Station ID. Could probably be unsigned
                int  id;
		// Station wall locations
		double up, down, left, right;
		//Station position relative to ROOT_STN
	        double x, y, z;
		// Position error relative to root
		// eT is total error
		double ex, ey, ez, eT;

	public:
		//Pointer to next station in linked list
		Station* nextStation;

		//Constructor
		Station();

		// Prints 'this' station. Used for debugging
		void print();

		// Prints all stations after 'this' in linked list
		// Use ROOT_STN to print all stations
		void printStations();

		// Insert the next station into the correct location
		Station* createNext(int, std::string, std::string, std::string, std::string);

		// Return pointer to station matching 'ID' or to location it should be inserted
		Station* find(int);

		// initialize the station with the parameters
		void setStation(int, double, double, double, double);

		// (re)position 'this' station based off the Path and it's errors
		void weightedAve( std::vector<double> Path );

		void resetStns();

		// Find the max and min of all cartesian coordinates
		void maxPos(double*, double*, double*, double*, double*, double*);

		// Count the number of stations
		unsigned int numStn();

		//Read the stations relative position
		void readPos(double*, double*, double*);

		//Read the stations relative Error
		void readError(double*, double*, double*);

		//Returns the total error of the station
		double totalError();

		// Set the stations position
		void setPos(double, double, double);

		// Set the stations Error
		void setError(double, double, double);

		// Return the station ID
		int readID();

}; // Station

// Create global variable. Pointer to ROOT_STN
extern Station* ROOT_STN;

#endif // STATION_H
