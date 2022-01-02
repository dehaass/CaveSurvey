#include "Splay.h"

using namespace std;

// Initialize root station
Splay* ROOT_SPLAY = new Splay();

//Constructor. Initializes everything to 0 or nullptr
Splay::Splay(){
    fromStn = nullptr;
    dx = dy = dz = 0;
}

// Constructor. Initializes from UDLR values
Splay::Splay(double x, double y, double z, Station *stn){
    //Splay* prevSplay;
    //fromStn = nullptr;
    //dx = dy = dz = 0;
    dx = x;
    dy = y;
    dz = z;
    fromStn = stn;
    //cout << "Station ID: " << stn->readID() << '\n';
    //stn->print();
}

// Prints 'this' splay. Used for debugging
void Splay::print(){
    cout << "From: " << fromStn->readID() << " dx: " << dx << " dy: " << dy << " dz: " << dz << endl;
} //end print

