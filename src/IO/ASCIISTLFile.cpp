#include <IO/ASCIISTLFile.h>


// De-serialisation primitive
std::istream& operator>>(std::istream & stream, Vertex& v) {
    stream >> v.x;
    stream >> v.y;
    stream >> v.z;
    return stream;
}

void ASCIISTLFile::parse(std::istream & dataFile) {
    std::string word;
    std::string title;
    Vertex vertex1, vertex2, vertex3, vertexIgnored;
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
                dataFile >> vertexIgnored;
                vertexIgnored.normer();
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
            dataFile >> vertex1;
            dataFile >> word;
            expectKeyword("vertex", word);
            dataFile >> vertex2;
            dataFile >> word;
            expectKeyword("vertex", word);
            dataFile >> vertex3;
            vectTriangle.push_back(Triangle(vertex1, vertex2, vertex3, vertexIgnored, 0, 0, 255));
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
