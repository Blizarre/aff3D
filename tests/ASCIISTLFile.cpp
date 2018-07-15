#include "IO/ASCIISTLFile.h"
#include "fileParser.h"
#include "sstream"
#include "string"
#include <gtest/gtest.h>
#include <iostream>
#include <istream>

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

std::string validSTL2 = R"n0(solid This is a big file
  facet normal 4 5 6  
  outer loop
    vertex 01 02 03   
    vertex 11 12 13
    vertex 21 22 23
  endloop
  endfacet
  facet normal 4 5  6
  outer loop
    vertex 01 02 03   
    vertex 11 12 13
    vertex 21 22 23
  endloop
  endfacet
endsolid 
)n0";

std::string validSTL3 = R"n0(solid
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
std::string validSTL2_description{"This is a big file"};
std::string validSTL3_description{""};

std::array<Vertex, 3> validSTL1_vertexes{
    Vertex{1, 2, 3}, Vertex{11, 12, 13}, Vertex{21, 22, 23},
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
  // simple one-word description, one triangle
  std::istringstream validStream{validSTL1};
  ASCIISTLFile file(validStream);
  std::vector<Triangle> triangles = file.triangles();
  EXPECT_EQ(triangles.size(), 1u);
  EXPECT_EQ(file.description(), validSTL1_description);
  EXPECT_EQ(triangles[0].points, validSTL1_vertexes);

  // description with whitespaces, several triangles
  std::istringstream validStream2{validSTL2};
  ASCIISTLFile file2(validStream2);
  std::vector<Triangle> triangles2 = file2.triangles();
  EXPECT_EQ(triangles2.size(), 2u);
  EXPECT_EQ(file2.description(), validSTL2_description);

  // no description
  std::istringstream validStream3{validSTL3};
  ASCIISTLFile file3(validStream3);
  std::vector<Triangle> triangles3 = file3.triangles();
  EXPECT_EQ(triangles3.size(), 1u);
  EXPECT_EQ(file3.description(), validSTL3_description);
}

TEST(ASCIISTLFile, failInvalidFileType) {
  std::istringstream notSTLStream{notSTL};
  EXPECT_THROW(ASCIISTLFile{notSTLStream}, InvalidFileType);
}

TEST(ASCIISTLFile, failInvalidFileContent) {
  std::istringstream invalidSTLStream{invalidSTL1};
  EXPECT_THROW(ASCIISTLFile{invalidSTLStream}, ParseError);
}
}
