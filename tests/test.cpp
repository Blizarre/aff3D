#include <gtest/gtest.h>
#include "sstream"
#include "string"
#include <istream>
#include "IO/ASCIISTLFile.h"
#include "fileParser.h"
#include <iostream>

namespace tests {

std::string validSTL1 = R"n0(solid test0
  facet normal 4 5 6  
  outer loop
    vertex 01 02 03   
    vertex 11 12 13
    vertex 21 22 23
  endloop
  endfacet
endsolid 
)n0";

  std::string validSTL1_description{"test0"};

std::array<Vertex, 3> validSTL1_vertexes {
  Vertex{1, 2, 3},
  Vertex{11, 12, 13},
  Vertex{21, 22, 23},
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
