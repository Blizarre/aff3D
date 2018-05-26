#include "IO/FileParser.h"
#include "IO/ASCIISTLFile.h"

std::unique_ptr<FileParser> FileParser::getParser(const std::string &fileName) {
  if (ASCIISTLFile::canParse(fileName)) {
    return std::make_unique<ASCIISTLFile>(fileName);
  }
  throw new ParseError("Cannot find a suitable Parser");
}
