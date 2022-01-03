#include "Splay.h"

using namespace std;

// Initialize root station
Splay* ROOT_SPLAY = new Splay();

//Constructor. Initializes everything to 0 or nullptr
Splay::Splay(){
    dx = dy = dz = 0;
}

// Constructor. Initializes from UDLR values
Splay::Splay(double x, double y, double z){
    //dx = dy = dz = 0;
    dx = x;
    dy = y;
    dz = z;
    //cout << "Station ID: " << stn->readID() << '\n';
    //stn->print();
}

// Prints 'this' splay with one float point precision
void Splay::print(){
    //cout << "Splay: x= " << std::setprecision(1) << dx << " y= " << std::setprecision(1) << dy << " z= " << std::setprecision(1) << dz << '\n';
    //cout << std::fixed;
    //cout << std::setprecision(1);
    cout << " dx: " << dx << " dy: " << dy << " dz: " << dz << endl;
} //end print

// gets xyz values of the splay
void Splay::readDeltas(double *x, double *y, double *z){
    *x = dx;
    *y = dy;
    *z = dz;
}