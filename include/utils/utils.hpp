#pragma once

#include <string>
#include <vector>

class Path {
  std::string path;

#ifdef VS2015
#define data_separator_def '\\'
#else
#define data_separator_def '/'
#endif

public:
  Path* Add(const std::string pathToAdd);
  Path* Pop();
  operator const char*() const;
};

std::vector<std::string> Split(std::string str, char separator = ' ');