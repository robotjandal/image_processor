#ifndef CMAKE_IMAGEPARSE_H
#define CMAKE_IMAGEPARSE_H

#include <map>
#include <string>
#include <vector>

namespace ImageProcessor {

// to specify case returned by get_keys()
enum class KEYS_CASE : unsigned char {
  E_LOWERCASE,
  E_UPPERCASE,
};

// String wrapper with easy conversions to integer, floating point
// or boolean values
class Parameter {
public:
  Parameter(){};
  explicit Parameter(std::string const value) : value_{value} {};
  std::string get_string() const { return value_; };
  int get_int() const;
  float get_float() const;
  bool get_bool() const;

private:
  std::string value_;
};

// map of strings wrapper to hold content of action's from an external
// source (e.g. YAML file). Key's default to lowercase
class ParseMap {
public:
  ParseMap(){};
  explicit ParseMap(std::map<std::string, Parameter> const arguments)
      : arguments_{arguments} {};
  Parameter operator[](std::string const key) const;

  int size() const { return arguments_.size(); };
  void add(std::string const key, std::string const value);
  void remove(std::string const key);
  std::pair<std::string, Parameter> find(std::string const key) const;
  std::vector<std::string> get_keys(KEYS_CASE const keys_case) const;

private:
  std::map<std::string, Parameter> arguments_;
};

} // namespace ImageProcessor
#endif