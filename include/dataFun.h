#ifndef DATAFUN_H
#define DATAFUN_H

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "Shot.h"
#include "Station.h"

using std::cout;

const bool TRUE=1;
const bool FALSE=0;

// Convert a string to double with error checking
double strToF(std::string);

// Convert a string to int with error checking
int strToL(std::string);

// Read through file and input stations in order
int populateStations(std::ifstream&);

// Read through file and input Shots in order
int populateShots(std::ifstream&);

#endif // DATAFUN_H
