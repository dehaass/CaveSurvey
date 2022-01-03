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
    void ShotList(unsigned int **shotList, const unsigned int numShots){
        Shot* currShot = ROOT_SHOT;
        for(unsigned int i = 0; i < numShots; i++){
            shotList[i][0] = currShot->readFromStnID();
            shotList[i][1] = currShot->readToStnID();
            currShot = currShot->nextShot;
            //cout << "ShotList: " << shotList[i][0] << " " << shotList[i][1] << "\n";
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

extern "C"{
    //Creates mesh data for cave walls from udlr station data and shots
    void tunnelMesh(unsigned int **verts, unsigned int **edges, unsigned int **faces, const unsigned int numShots){
        Shot* currShot = ROOT_SHOT;
        for(unsigned int i = 0; i < numShots; i++){

// Doesn't work. TODO
            verts[i][0] = currShot->readFromStn()->getX();
            verts[i][1] = currShot->readFromStn()->getY();
            verts[i][2] = currShot->readFromStn()->getZ();

            currShot = currShot->nextShot;
            if(currShot == nullptr){
                break;
            }
        }
    }
}


extern "C"{
    // populates a list of vertices and edges for splay shots from all stations
    void splayEdgesGenerator(double **verts, const unsigned int sizeofVerts, unsigned int **edges, const unsigned sizeofEdges){
        Station* currStation = ROOT_STN;
        std::vector<Splay*>* splayList;
        double x, y, z, dx, dy, dz;
        std::vector<Splay*> tempSplayList;

        unsigned int i, j, k = 0;

        while( currStation != nullptr){
            currStation->print();
            currStation->readPos(&x, &y, &z);
            cout << "It's gonna break here\n";
            if(verts[0][0] == 1){
                cout << "if true\n";
            }else{
                cout << "if not true\n";
            }
            cout << verts[0][0];
            cout << "It broke here";
            verts[i][0] = x;
            verts[i][1] = y;
            verts[i][2] = z;
            i++;
            if(i > sizeofVerts) break;

            splayList = currStation->getSplayList();
            cout << "Didn't break\n";
            for(j=0; j<splayList->size(); j++){
                tempSplayList = *splayList;
                tempSplayList[j]->readDeltas(&dx, &dy, &dz);
                verts[i][0] = x + dx;
                verts[i][1] = y + dy;
                verts[i][2] = z + dz;
                edges[k][0] = i-1-j;
                edges[k][1] = i;
                k++;
                if(k > sizeofEdges) return;
                i++;
                if(i > sizeofVerts) return;

            }

            currStation = currStation->nextStation;
        }
    }
}