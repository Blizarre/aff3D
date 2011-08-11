
#include "IO.h"

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
                v2[i] = Vertex(x,y,-z);;
            } else {
                v[i] = Vertex(x/100.0 -0.8,0.5 - y/100.0 ,z/100.0-1 );
                v2[i] = Vertex(x/100.0 -0.8,0.5 - y/100.0 ,(-z)/100.0-1 );
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
    
    cout <<"Triangles lu : " <<vectTriangle.size() <<endl;
}
