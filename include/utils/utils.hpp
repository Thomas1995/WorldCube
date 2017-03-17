#pragma once

#include <string>

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
