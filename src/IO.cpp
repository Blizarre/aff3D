
#include "IO.h"
#include <fstream>
#include <array>
#include <stdexcept>
#include <cassert>
#include <limits>


class STLFileParser {
public:
	STLFileParser(const string& fileName)
	{
		ifstream dataFile(fileName);

		if (!dataFile)
			throw invalid_argument("Error opening the file");

		std::string word;
		std::string title;
		Vertex a, b, c, normal;
		State state = State::SOLID;
		// File format:
		//
		// solid name
		//  facet normal ni nj nk
		//   outer loop
		//    vertex v1x v1y v1z
		//    vertex v2x v2y v2z
		//    vertex v3x v3y v3z
		//   endloop
		//  endfacet
		//  facet normal ...
		//   outer loop
		//   ...
		//   endloop
		//  endfacet
		// endsolid

		// tokenizer: all tokens are separated by space
		while (dataFile >> word) {
			switch (state) {
			case State::SOLID:
				expectKeyword("solid", word);
				dataFile >> title;
				state = State::FACET;
				break;
			case State::FACET:
				if (word == "endsolid") {
					state = State::END_SOLID;
				}
				else {
					expectKeyword("facet", word);
					dataFile >> word;
					expectKeyword("normal", word);
					normal = readVertex(dataFile);
					normal.normer();
					state = State::OUTER_LOOP;
				}
				break;
			case State::OUTER_LOOP:
				expectKeyword("outer", word);
				dataFile >> word;
				expectKeyword("loop", word);
				state = State::VERTEX_LIST;
				break;
			case State::VERTEX_LIST:
				expectKeyword("vertex", word);
				a = readVertex(dataFile);
				dataFile >> word;
				expectKeyword("vertex", word);
				b = readVertex(dataFile);
				dataFile >> word;
				expectKeyword("vertex", word);
				c = readVertex(dataFile);
				vectTriangle.push_back(Triangle(a, b, c, normal, 0, 0, 255));
				state = State::END_LOOP;
				break;
			case State::END_LOOP:
				expectKeyword("endloop", word);
				dataFile >> word;
				expectKeyword("endfacet", word);
				state = State::FACET;
				break;
			case State::END_SOLID:
				break;
			}
		}
	}

	const vector<Triangle> triangles() {
		return vectTriangle;
	}

private:

	vector<Triangle> vectTriangle;

	void expectKeyword(std::string expected, std::string read) {
		if (expected != read) {
			throw runtime_error("STL: Expect keyword '" + expected + "', got " + read);
		}
	}

	enum class State {
		SOLID, FACET, OUTER_LOOP, VERTEX_LIST, END_LOOP, END_SOLID
	};

	Vertex readVertex(ifstream& stream) {
		float x, y, z;
		stream >> x;
		stream >> y;
		stream >> z;
		return Vertex(x, y, z);
	}
};

class TriangleNormalizer {
public:

	template <class Iter>
	TriangleNormalizer(Iter begin, Iter end) {
		while (begin != end) {
			for (int i = 0; i < 3; i++) {
				minX = MIN(minX, begin->points[i].x);
				minY = MIN(minY, begin->points[i].y);
				minZ = MIN(minZ, begin->points[i].z);
				maxX = MIN(maxX, begin->points[i].x);
				maxY = MIN(maxY, begin->points[i].y);
				maxZ = MIN(maxZ, begin->points[i].z);
			}
			begin++;
		}
		scale = MAX3(maxX - minX, maxY - minY, maxZ - minZ);
	}

	template <class RandomAccessIterator>
	vector<Triangle> normalize(RandomAccessIterator begin, RandomAccessIterator end) {
		vector<Triangle> normalized(end - begin);
		while (begin != end) {
			Triangle t = *begin;
			for (int i = 0; i < 3; i++) {
				Vertex& v = t.points[i];
				v.x -= (maxX - minX) / 2.0f;
				v.y -= (maxY - minY) / 2.0f;
				v.z -= (maxZ - minZ) / 2.0f;

				v.x /= scale;
				v.y /= scale;
				v.z /= scale;
			}
			normalized.push_back(t);
			begin++;
		}
		return normalized;
	}

private:
	float minX = numeric_limits<float>::max();
	float maxX = numeric_limits<float>::min();
	float minY = numeric_limits<float>::max();
	float maxY = numeric_limits<float>::min();
	float minZ = numeric_limits<float>::max();
	float maxZ = numeric_limits<float>::min();

	float scale;
};

/**
 * Read the file `fileName` and fill the vectTriangle vector with the Triangles.
 * The processing will throw a std::string on error. 
 **/
vector<Triangle> readFromFile(const string& fileName) {
	STLFileParser file(fileName);
	auto triangles = file.triangles();
	TriangleNormalizer normalizer(triangles.cbegin(), triangles.cend());
	return normalizer.normalize(triangles.cbegin(), triangles.cend());
}
