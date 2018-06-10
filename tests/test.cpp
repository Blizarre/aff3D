#include <gtest/gtest.h>
#include "sstream"
#include "string"
#include <istream>
#include "IO/ASCIISTLFile.h"
#include "fileParser.h"
#include <iostream>

namespace tests {

std::string validSTL1 = R"n0(solid test0
  facet normal 1 0 0
  outer loop
    vertex 1 0 0
    vertex 0 1 0
    vertex 0 0 1
  endloop
  endfacet
endsolid
)n0";

  std::string validSTL1_description{"test0"};

std::array<Vertex, 4> validSTL1_vertexes {
  Vertex{1, 0, 0},
  Vertex{0, 1, 0},
  Vertex{0, 0, 1},
  Vertex{1, 0, 0}
};

std::string notSTL = R"n0(not a
STL file)n0";

std::string invalidSTL1 = R"n0(solid test0
  facet normal 1 2 3
  outer loop
    vertex  22 33
    vertex 21 22 23
    vertex 31 32 33
  endloop
  endfacet
endsolid
)n0";

TEST(ASCIISTLFile, openSimpleSTL) {
  std::istringstream validStream{validSTL1};
  ASCIISTLFile file(validStream);
  std::vector<Triangle> triangles = file.triangles();
  ASSERT_EQ(triangles.size(), 1u);
  EXPECT_EQ(file.description(), validSTL1_description);
  EXPECT_EQ(triangles[0].points, validSTL1_vertexes);
}

TEST(ASCIISTLFile, failInvalidFileType) {
  std::istringstream notSTLStream{notSTL};
  ASSERT_THROW(ASCIISTLFile{notSTLStream}, InvalidFileType);
}

TEST(ASCIISTLFile, failInvalidFileContent) {
  std::istringstream invalidSTLStream{invalidSTL1};
  ASSERT_THROW(ASCIISTLFile{invalidSTLStream}, ParseError);
}

}
