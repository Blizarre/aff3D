#include "fileParser.h"
#include "ASCIISTLFile.h"
#include "BINSTLFile.h"

#include <cerrno>
#include <cstring>
#include <fstream>
#include <iostream>

std::unique_ptr<FileParser> FileParser::getParser(const std::string &fileName) {
  {
    std::ifstream file{ fileName };
    if (!file.good()) {
      throw ParseError(std::string("Cannot open ") + fileName + ", " +
        std::strerror(errno));
    }
    try {
      return std::make_unique<ASCIISTLFile>(file);
    }
    catch (const InvalidFileType &e) {
    }
  }
  // If the file couldn't be opened as an ASCIISTLFile, we try the
  // binary version. Reseting the ifstream didn't always work,
  // So I re-opened it instead.
  std::ifstream file{ fileName, std::ios::binary };

  return std::make_unique<BINSTLFile>(file);
}
