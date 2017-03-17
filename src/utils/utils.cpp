#include "utils/utils.hpp"

// class Path

Path* Path::Add(const std::string pathToAdd) {
  if(!path.empty())
    path.push_back(data_separator_def);
  path += pathToAdd;
  return this;
};

Path* Path::Pop() {
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

Path::operator const char*() const { return path.c_str(); }
