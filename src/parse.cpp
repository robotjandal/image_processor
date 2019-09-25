
#include <boost/algorithm/string.hpp>
#include <sstream>
#include <stdexcept>

#include "exceptions.hpp"
#include "parse.hpp"

namespace ImageProcessor {

// convert string to int
int Parameter::get_int() const {
  std::stringstream ss;
  int output;
  ss << value_;
  ss >> output;
  if (ss.fail())
    throw ImageProcessorError("Error: Casting to int");
  return output;
}

// convert string to float
float Parameter::get_float() const {
  std::stringstream ss;
  float output;
  ss << value_;
  ss >> output;
  if (ss.fail())
    throw ImageProcessorError("Error: Casting to float");
  return output;
}

// convert string to bool
bool Parameter::get_bool() const {
  std::string output = boost::algorithm::to_lower_copy(value_);
  if (output == "true")
    return true;
  else if (output == "false")
    return false;
  else
    throw ImageProcessorError("Error: Casting to boolean");
}

void ParseMap::add(std::string key, std::string value) {
  arguments_.insert(std::pair<std::string, Parameter>(key, Parameter{value}));
}

Parameter &ParseMap::operator[](std::string key) { return arguments_.at(key); }

// Find returns both the key and value as a pair
std::pair<std::string, Parameter> ParseMap::find(std::string key) {
  auto output = arguments_.find(key);
  if (output != arguments_.end())
    return std::pair<std::string, Parameter>(output->first, output->second);
  else
    throw std::out_of_range("Key not found");
}

} // namespace ImageProcessor
