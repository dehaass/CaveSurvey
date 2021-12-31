#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "Shot.h"
#include "Station.h"
#include "dataFun.h"
#include "surveyPlot.h"
#include "svgMap.h"

using std::cout;
using std::cin;

// Main menu program
int menu();

// Debugging program. Bypasses user input
extern "C"{
    int giver();
}

// Read survey from '*.csv' file
int fromFile();

// Read user input to produce survey
int fromSI();

// Function for reading user input and checking for validity
// Will iterate message and error messages until correct input is recieved
int readNumSI(std::string, std::string*);

#endif // USERINTERFACE_H
