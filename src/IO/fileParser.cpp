#include "fileParser.h"
#include "ASCIISTLFile.h"

std::unique_ptr<FileParser> FileParser::getParser(const std::string &fileName) {
  if (ASCIISTLFile::canParse(fileName)) {
    return std::unique_ptr<ASCIISTLFile>(new ASCIISTLFile(fileName));
  }
  throw new ParseError("Cannot find a suitable Parser");
}
