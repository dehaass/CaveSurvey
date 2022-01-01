#include "blenderoutputs.h"


//typedef struct stationList {
//    double array[43][3];
//} stationList; 

extern "C"{
    // Returns a vector containing the x,y,z coordinates of all stations
    void StationLocations(double **stnList, const unsigned int numStn){


        Station* currStation = ROOT_STN;

        for(unsigned int i = 0; i < numStn; i++){
            stnList[i][0] = currStation->getX();
            stnList[i][1] = currStation->getY();
            stnList[i][2] = currStation->getZ();

            currStation = currStation->nextStation;
            if(currStation == nullptr){
                break;
            }
        }

        return ;
    }
}

extern "C"{
    // Counts the number of stations
    unsigned int numStns(){
        unsigned int num = 0;
        Station* currStation = ROOT_STN;
        while(currStation != nullptr){
            num++;
            currStation = currStation->nextStation;
        }
        return num;
    }
}

extern "C"{
    // Populates a shot list with from - to station IDs
    void ShotList(int **shotList, const unsigned int numShots){
        Shot* currShot = ROOT_SHOT;
        for(unsigned int i = 0; i < numShots; i++){
            shotList[i][0] = currShot->readFromStnID();
            shotList[i][1] = currShot->readToStnID();
            currShot = currShot->nextShot;
            if(currShot == nullptr){
                break;
            }
        }
        return;
    }
}

extern "C"{
    // Counts the number of shots and returns an unsigned int
    unsigned int numShots(){
        unsigned int num = 0;
        Shot* currShot = ROOT_SHOT;
        while(currShot != nullptr){
            num++;
            currShot = currShot->nextShot;
        }
        return num;
    }
}