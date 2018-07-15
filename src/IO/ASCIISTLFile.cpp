#include "ASCIISTLFile.h"

#include <string>

float readFloat(std::istream &is) {
  float f;
  if (!(is >> f)) {
    throw ParseError(
        std::string("Invalid floating point number detected at position ") +
        std::to_string(is.tellg()));
  }
  return f;
}

// De-serialisation primitive
std::istream &operator>>(std::istream &stream, Vertex &v) {
  v.x = readFloat(stream);
  v.y = readFloat(stream);
  v.z = readFloat(stream);
  return stream;
}

void ASCIISTLFile::parse(std::istream &dataFile) {
  std::string word;
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
      if (word != "solid") {
        throw InvalidFileType("Does not start with the keyword 'solid'");
      }
      // left trimming the description as it will contain spacing character
      std::getline(dataFile, _description);
      _description.erase(0, _description.find_first_not_of(" "));
      state = State::FACET;
      break;
    case State::FACET:
      if (word == "endsolid") {
        state = State::END_SOLID;
      } else {
        expectKeyword("facet", word);
        dataFile >> word;
        expectKeyword("normal", word);
        dataFile >> vertexIgnored;
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
      _triangles.push_back(Triangle(vertex1, vertex2, vertex3));
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
