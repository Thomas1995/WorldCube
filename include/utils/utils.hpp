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
  Path* Add(const std::string pathToAdd) {
    if(!path.empty())
      path.push_back(data_separator_def);
    path += pathToAdd;
    return this;
  };

  Path* Pop() {
    if(path.find(data_separator_def) != std::string::npos) {
      while(path.back() != data_separator_def)
        path.pop_back();
      path.pop_back();
    }
    else {
      path.clear();
    }
    return this;
  }

  operator const char*() const { return path.c_str(); }
};
