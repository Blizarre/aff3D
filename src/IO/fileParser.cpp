#include "fileParser.h"
#include "ASCIISTLFile.h"
#include "BINSTLFile.h"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>

std::unique_ptr<FileParser> FileParser::getParser(const std::string &fileName) {
  std::ifstream file{fileName};
  if (!file.good()) {
    throw ParseError(std::string("Cannot open ") + fileName + ", " +
                     std::strerror(errno));
  }
  try {
    return std::make_unique<ASCIISTLFile>(file);
  } catch (const InvalidFileType &e) {
  }
  file.seekg(0, std::ios::beg);
  return std::make_unique<BINSTLFile>(file);
}
