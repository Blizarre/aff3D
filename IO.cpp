
#include "IO.h"

#define MAX(a,b)    ((a)>(b)?(a):(b))
#define MAX3(a,b,c) (MAX( MAX((a),(b)), (c)))

void readFromFile(string fileName, vector<Triangle> & vectTriangle) {

    char dummy[200];
    FILE* fd = fopen(fileName.c_str(), "r");
    
    if(!fd) {
        cerr << "Erreur d'ouverture du fichier" << fileName <<endl;
        exit(0);
    }
    float x,y,z,r,g,b;
    int nbLu = 0;
    
    Vertex v[3], v2[3];
    
    bool erreur = false;
    while( ! feof(fd) ) {
        for(int i=0; i<4;i++) { // x y z norm
            nbLu = fscanf(fd, "%f %f %f", &x, &y, &z);
            if(nbLu != 3) 
                erreur = true;
            if(i==0) {
                v[i] = Vertex(x,y,z);
                v2[i] = Vertex(x,y,z); 
            } else {
                v[i]  = Vertex(x, y, z);
                v2[i] = Vertex(x, y, - z); // -z car seconde moitiée d la théière
            }
        }
        if(! erreur) {
            v[0].normer();
            v2[0].normer();
            vectTriangle.push_back( Triangle(v[1], v[2], v[3], v[0], 0,0,255) );
            vectTriangle.push_back( Triangle(v2[1], v2[2], v2[3], v2[0], 0,0,255) );
        }
    }

    fclose(fd);
    
    cout << "scaling model" <<endl;
    vector<Triangle>::iterator tr;
    float minX = 100000, maxX = -100000, minY = 100000, maxY = -100000, minZ = 100000, maxZ = -100000;
    int numVert;
    for(tr = vectTriangle.begin(); tr != vectTriangle.end(); tr++) {
        for(numVert = 0; numVert < 3; numVert ++) {
            if(tr->points[numVert].x < minX) minX = tr->points[numVert].x;
            if(tr->points[numVert].y < minY) minY = tr->points[numVert].y;
            if(tr->points[numVert].z < minZ) minZ = tr->points[numVert].z;
            if(tr->points[numVert].x > maxX) maxX = tr->points[numVert].x;
            if(tr->points[numVert].y > maxY) maxY = tr->points[numVert].y;
            if(tr->points[numVert].z > maxZ) maxZ = tr->points[numVert].z;
        }
    }
    cout <<"minX: "<< minX<< ", maxX: " << maxX <<endl;
    
    float coeffEchelle = MAX3(maxX-minX, maxY-minY, maxZ-minZ);
    
    for(tr = vectTriangle.begin(); tr != vectTriangle.end(); tr++) {
        for(numVert = 0; numVert < 3; numVert ++) {
            tr->points[numVert].x -= (maxX - minX)/2.0;
            tr->points[numVert].y -= (maxY - minY)/2.0;
            tr->points[numVert].z -= (maxZ - minZ)/2.0;
            
            tr->points[numVert].x /= coeffEchelle;
            tr->points[numVert].y /= coeffEchelle;
            tr->points[numVert].z /= coeffEchelle;
        }
    }
    
    cout <<"Triangles lu : " <<vectTriangle.size() <<endl;
}
