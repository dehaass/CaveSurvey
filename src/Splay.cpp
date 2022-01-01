#include "Splay.h"

using namespace std;

// Initialize root station
Splay* ROOT_SPLAY = new Splay();

//Constructor. Initializes everything to 0 or nullptr
Splay::Splay(){
    fromStn = nullptr;
    dx = dy = dz = 0;
}