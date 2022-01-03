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
    // populates a list of vertices and edges for splay shots from all stations
    void splayEdgesGenerator(double **verts, const unsigned int sizeofVerts, unsigned int **edges, const unsigned sizeofEdges){
        Station* currStation = ROOT_STN;
        std::vector<Splay*>* splayList;
        double x, y, z, dx, dy, dz;
        std::vector<Splay*> tempSplayList;

        unsigned int i = 0;
        unsigned int j = 0;
        unsigned int k = 0;

        while( currStation != nullptr){
            currStation->print();
            currStation->readPos(&x, &y, &z);
            //cout << "It's gonna break here\n";
            verts[i][0] = x;
            verts[i][1] = y;
            verts[i][2] = z;
            i++;
            if(i >= sizeofVerts) break;

            splayList = currStation->getSplayList();
            tempSplayList = *splayList;
            for(j=0; j<tempSplayList.size(); j++){
                //tempSplayList = *splayList;
                tempSplayList[j]->readDeltas(&dx, &dy, &dz);
                verts[i][0] = x + dx;
                verts[i][1] = y + dy;
                verts[i][2] = z + dz;
                edges[k][0] = i-1-j;
                edges[k][1] = i;
                k++;
                if(k >= sizeofEdges) return;
                i++;
                if(i >= sizeofVerts) return;

            }

            currStation = currStation->nextStation;
        }
    }
}

// Simulates the python script that allocates the memory for the splay list
void testSplayEdgeGenerator(){
    cout << "Begin testSplayEdgeGenerator\n";

    unsigned int Stns = numStns();
    unsigned int numVerts = Stns*5;
    unsigned int numEdges = Stns*4;
    //double verts[numVerts][3] = {0};
    //double edges[numEdges][2] = {0};
    static double** verts = new double*[numVerts];
    for(unsigned int i = 0; i < numVerts; i++){
        verts[i] = new double[3];
        for(unsigned int j = 0; j < 3; j++){
            verts[i][j] = 0;
        }
    }
    static unsigned int** edges = new unsigned int*[numEdges];
    for(unsigned int i = 0; i < numEdges; i++){
        edges[i] = new unsigned int[2];
        for(unsigned int j = 0; j < 2; j++){
            edges[i][j] = 0;
        }
    }
    cout << verts[0][0] << "\n";
    splayEdgesGenerator(verts, numVerts, edges, numEdges);
}