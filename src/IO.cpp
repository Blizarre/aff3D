
#include "IO.h"
#include <fstream>

void readTriangles(string& fileName, vector<Triangle> & vectTriangle)
{
	// TODO: fd not released in case of exception, use RAII
	ifstream dataFile(fileName);
	float x, y, z;

	Vertex v[4], v2[4];
	int nbLu = 0;

	while (!dataFile.eof()) {
		for (int i = 0; i<4; i++) { // normale du triangle, puis points 1, 2, 3 du triangle
			dataFile >> x >> y >> z;
			if (dataFile.fail())
			{
				throw exception("Error reading data from file");
			}
			v[i] = Vertex(x, y, z);
			v2[i] = Vertex(x, y, -z); // -z car le fichier ne contient qu'une moitiée de théière. v2 représente l'autre moitiée
		}
		v[0].normer();
		v2[0].normer();
		vectTriangle.push_back(Triangle(v[1], v[2], v[3], v[0], 0, 0, 255));       // point 1, 2, 3, puis normale
		vectTriangle.push_back(Triangle(v2[1], v2[2], v2[3], v2[0], 0, 0, 255));
	}
}

/**
 * Lit le fichier de données depuis le fichier filename. pour l'instant, le format est 
 * non documenté (STL reformaté). A améliorer pour prendre en compte le STL et les erreurs.
 * Renvoie vrai si le fichier est lu. 
 **/
bool readFromFile(string fileName, vector<Triangle> & vectTriangle) {
    

    
    
	readTriangles(fileName, vectTriangle);

    
    cout << "scaling model" <<endl;

    float minX = 100000, maxX = -100000, minY = 100000, maxY = -100000, minZ = 100000, maxZ = -100000;
    int numVert;
    for(auto& tr : vectTriangle) {
        for(numVert = 0; numVert < 3; numVert ++) {
            if(tr.points[numVert].x < minX) minX = tr.points[numVert].x;
            if(tr.points[numVert].y < minY) minY = tr.points[numVert].y;
            if(tr.points[numVert].z < minZ) minZ = tr.points[numVert].z;
            if(tr.points[numVert].x > maxX) maxX = tr.points[numVert].x;
            if(tr.points[numVert].y > maxY) maxY = tr.points[numVert].y;
            if(tr.points[numVert].z > maxZ) maxZ = tr.points[numVert].z;
        }
    }
    cout <<"minX: "<< minX<< ", maxX: " << maxX <<endl;
    
    float coeffEchelle = MAX3(maxX-minX, maxY-minY, maxZ-minZ);
    
    for(auto& tr : vectTriangle) {
        for(numVert = 0; numVert < 3; numVert ++) {
            tr.points[numVert].x -= (maxX - minX)/2.0f;
            tr.points[numVert].y -= (maxY - minY)/2.0f;
            tr.points[numVert].z -= (maxZ - minZ)/2.0f;
            
            tr.points[numVert].x /= coeffEchelle;
            tr.points[numVert].y /= -coeffEchelle; // le "-" permer d'avoir la théière dans le bon sens (couvercle vers le haut)
            tr.points[numVert].z /= coeffEchelle;
        }
        tr.points[3].y = - tr.points[3].y; // De même avec la normale
    }
    
    cout <<"Triangles lu : " <<vectTriangle.size() <<endl;

    return true;
}
