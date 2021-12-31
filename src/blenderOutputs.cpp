#include "blenderoutputs.h"


//typedef struct stationList {
//    double array[43][3];
//} stationList; 

extern "C"{
    // Returns a vector containing the x,y,z coordinates of all stations
    void StationLocations(double **stnList, const unsigned int numStn){

        //unsigned int numStn = numStns();

        //double stnList[numStn][3] = {};
        //std::vector<double> stnList;

        Station* currStation = ROOT_STN;
        //while(currStation != nullptr){
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