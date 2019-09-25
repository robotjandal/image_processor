#ifndef CMAKE_IMAGEPARSE_H
#define CMAKE_IMAGEPARSE_H

#include <map>
#include <string>

namespace ImageProcessor {

class Parameter {
public:
  explicit Parameter(std::string value) : value_{value} {};
  std::string get_string() const { return value_; };
  int get_int() const;
  float get_float() const;
  bool get_bool() const;

private:
  const std::string value_;
};

// map of strings wrapper
class ParseMap {
public:
  // explicit ParseMap();
  void add(std::string key, std::string value);
  Parameter &operator[](std::string key);
  int size() { arguments_.size(); };
  std::pair<std::string, Parameter> find(std::string key);

private:
  std::map<std::string, Parameter> arguments_;
};

} // namespace ImageProcessor
#endif