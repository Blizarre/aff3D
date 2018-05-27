#pragma once

#include <fstream>
#include <istream>

#include <string>
#include <vector>

#include <IO/fileParser.h>

class ASCIISTLFile : public FileParser {
public:
  ASCIISTLFile(const std::string &fileName) {
    std::ifstream dataFile(fileName);

    if (!dataFile)
      throw ParseError("Error opening the file");

    parse(dataFile);
  }

  virtual const std::vector<Triangle> triangles() { return vectTriangle; }

  static bool canParse(const std::string &fileName) {
    try {
      new ASCIISTLFile(fileName);
    } catch (std::runtime_error e) {
      return false;
    }
    return true;
  }

private:
  std::vector<Triangle> vectTriangle;

  void parse(std::istream &dataFile);

  void expectKeyword(std::string expected, std::string read) {
    if (expected != read) {
      throw std::runtime_error("STL: Expect keyword '" + expected + "', got " +
                               read);
    }
  }

  enum class State {
    SOLID,
    FACET,
    OUTER_LOOP,
    VERTEX_LIST,
    END_LOOP,
    END_SOLID
  };
};
