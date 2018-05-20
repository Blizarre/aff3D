#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Triangle.h"

class ParseError : public std::runtime_error {
public:
	ParseError(const std::string& msg) : std::runtime_error(msg) {}
};

class FileParser {
public:
	virtual const std::vector<Triangle> triangles() = 0;
	static std::unique_ptr<FileParser> getParser(const std::string& fileName);
};

