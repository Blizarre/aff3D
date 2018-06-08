#include "BINSTLFile.h"

#include <iostream>
#include <string>

template <typename T> T read(std::istream &is) {
  // TODO: Handle little/big endian. This code assume same endianness
  const size_t size = sizeof(T);
  T token;
  is.read(reinterpret_cast<char *>(&token), size);
  if (is.eof()) {
    throw new ParseError(std::string("End of File reached @") +
                         std::to_string(is.tellg()));
  } else if (is.fail()) {
    throw new ParseError("Error reading the file");
  }
  return token;
}

auto readf = [](std::istream &is) { return read<float>(is); };
auto readu16 = [](std::istream &is) { return read<uint16_t>(is); };

Vertex readVertex(std::istream &is) {
  return Vertex(readf(is), readf(is), readf(is));
}

std::string readDescription(std::istream &dataFile) {
  constexpr size_t descriptionSz{80};
  std::array<char, descriptionSz> description;
  dataFile.read(description.data(), descriptionSz);
  return std::string(description.data(), descriptionSz);
}

void BINSTLFile::parse(std::istream &dataFile) {
  Vertex vertex1, vertex2, vertex3, vertexIgnored;
  // File format: https://en.wikipedia.org/wiki/STL_(file_format)#Binary_STL
  //
  // UINT8[80] - Header
  // UINT32 - Number of triangles
  //
  // foreach triangle
  // REAL32[3] - Normal vector
  // REAL32[3] - Vertex 1
  // REAL32[3] - Vertex 2
  // REAL32[3] - Vertex 3
  // UINT16 - Attribute byte count
  // end
  _description = readDescription(dataFile);
  uint32_t szTriangles;
  szTriangles = read<uint32_t>(dataFile);
  std::cout << "Number of triangles " << szTriangles << std::endl;
  for (uint32_t i = 0; i < szTriangles; i++) {
    Vertex norm = readVertex(dataFile);
    Vertex a = readVertex(dataFile);
    Vertex b = readVertex(dataFile);
    Vertex c = readVertex(dataFile);
    readu16(dataFile);
    _triangles.push_back(Triangle(a, b, c, norm, 0, 0, 255));
  }
}
