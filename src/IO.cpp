
#include "IO.h"
#include <fstream>
#include <array>

void readTriangles(const string& fileName, vector<Triangle> & vectTriangle)
{
	// TODO: fd not released in case of exception, use RAII
	ifstream dataFile(fileName);
	float x, y, z;

	std::array<Vertex, 4> v, vMirror;

	if (!dataFile)
		throw exception("Error opening the file");

	while (!dataFile.eof()) {
		for (int i = 0; i<4; i++) { // normale du triangle, puis points 1, 2, 3 du triangle
			dataFile >> x >> y >> z;
			if (dataFile.fail())
			{
				throw exception("Error reading data from file");
			}
			v[i] = Vertex(x, y, z);
			vMirror[i] = Vertex(x, y, -z); // -z car le fichier ne contient qu'une moitiée de théière. v2 représente l'autre moitiée
		}
		v[0].normer();
		vMirror[0].normer();
		vectTriangle.push_back(Triangle(v[1], v[2], v[3], v[0], 0, 0, 255));       // point 1, 2, 3, puis normale
		vectTriangle.push_back(Triangle(vMirror[1], vMirror[2], vMirror[3], vMirror[0], 0, 0, 255));
	}
}

/**
 * Lit le fichier de données depuis le fichier filename. pour l'instant, le format est 
 * non documenté (STL reformaté). A améliorer pour prendre en compte le STL et les erreurs.
 **/
void readFromFile(const string& fileName, vector<Triangle> & vectTriangle) {
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
        // Make sure that the rawData of the Triangle is re-initialized with the correct values.
        // TODO: refactor this part of the code from scratch and implement a proper normalization/data loading code
        std::array<Vertex, 4> data = tr.points;
        for(numVert = 0; numVert < 3; numVert ++) {
            data[numVert].x -= (maxX - minX) / 2.0f;
            data[numVert].y -= (maxY - minY) / 2.0f;
            data[numVert].z -= (maxZ - minZ) / 2.0f;
            
            data[numVert].x /= coeffEchelle;
            data[numVert].y /= -coeffEchelle; // the "-" will revert the teapot upside-down
            data[numVert].z /= coeffEchelle;
        }
        data[3] = tr.points[3];
        data[3].y = -tr.points[3].y; // revert the normal also since the y component has been reverted

        tr.setRawData(data);
    }
    
    cout <<"Triangles lu : " <<vectTriangle.size() <<endl;
}
