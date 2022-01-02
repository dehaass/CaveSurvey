#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>
//#include <array>
//#include <vector>
#include "Station.h"
#include "Shot.h"
#include "dataFun.h"

class Station;
class Shot;

class Splay{

    friend class Station;
    friend class Shot;

    protected:


// Origin station of the shot
        //Station *fromStn;

// These are the relative cartesian coordinates wrt fromStn
        double dx, dy, dz;
    public:

// Let's not do a linked list. That's dumb
    //Splay* nextSplay;

    //Constructor
    Splay();

    Splay(double x, double y, double z);

    // Prints 'this' splay. Used for debugging
    void print();

    //Splay* createNextFromSpay( int, double, double, double );

    // reads the relative cartesian coordinates from the shot
    void readDeltas(double*, double*, double*);

    // Calulates the absolute cartesian coordinates of the shot
    void calcAbsCoords(double*, double*, double*);


};//end class Splay

extern Splay* ROOT_SPLAY;

#endif // SPLAY_H