#pragma once

#include <fstream>
#include <istream>

#include <string>
#include <vector>

#include "fileParser.h"

class ASCIISTLFile : public FileParser {
public:
  ASCIISTLFile(std::ifstream &data) { parse(data); }

  virtual const std::vector<Triangle> triangles() { return _triangles; }
  virtual const std::string description() { return _description; }

private:
  std::vector<Triangle> _triangles;
  std::string _description;

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
