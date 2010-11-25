
#include "IO.h"

void readFromFile(char* fileName, vector<Triangle> & vectTriangle) {

    FILE* fd = fopen(fileName, "r");
    
    if(!fd) {
        cerr << "Erreur d'ouverture du fichier" << fileName <<endl;
        exit(0);
    }
    float x,y,z,r,g,b;
    int nbLu = 0;
    
    Vertex v[3];
    
    bool erreur = false;
    while( ! feof(fd) ) {
        for(int i=0; i<3;i++) {
            nbLu = fscanf(fd, "%f %f %f", &x, &y, &z);
            if(nbLu != 3) 
                erreur = true;
            v[i] = Vertex(x,y,z);
        }
        fscanf(fd, "%f %f %f", &r, &g, &b);
        if(! erreur)
            vectTriangle.push_back( Triangle(v[0], v[1], v[2], r,g,b) );
    }

    fclose(fd);
    
    cout <<"Triangles lu : " <<vectTriangle.size() <<endl;
}
