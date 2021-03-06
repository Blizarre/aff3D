#pragma once

#include "triangle.h"
#include <memory>
#include <string>
#include <vector>

class ParseError : public std::runtime_error {
public:
  ParseError(const std::string &msg) : std::runtime_error(msg) {}
};

class InvalidFileType : public std::runtime_error {
public:
  InvalidFileType(const std::string &msg) : std::runtime_error(msg) {}
};

class FileParser {
public:
  virtual const std::vector<Triangle> triangles() = 0;
  virtual const std::string description() = 0;
  static std::unique_ptr<FileParser> getParser(const std::string &fileName);
};
