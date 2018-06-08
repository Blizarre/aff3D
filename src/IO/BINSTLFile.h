#pragma once

#include <fstream>
#include <istream>

#include <string>
#include <vector>

#include "fileParser.h"

class BINSTLFile : public FileParser {
public:
  BINSTLFile(std::ifstream &data) { parse(data); }

  virtual const std::vector<Triangle> triangles() { return _triangles; }
  virtual const std::string description() { return _description; }

private:
  std::vector<Triangle> _triangles;
  std::string _description;

  void parse(std::istream &is);
};
