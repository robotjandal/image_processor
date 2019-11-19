#include "parse.hpp"

#include <sstream>
#include <stdexcept>

#include "exceptions.hpp"
#include <boost/algorithm/string.hpp>

namespace ImageProcessor {

// Parameter
////
int Parameter::get_int() const {
  std::stringstream ss;
  int output;
  ss << value_;
  ss >> output;
  if (ss.fail())
    throw ImageProcessorError("Error: Casting to int");
  return output;
}

float Parameter::get_float() const {
  std::stringstream ss;
  float output;
  ss << value_;
  ss >> output;
  if (ss.fail())
    throw ImageProcessorError("Error: Casting to float");
  return output;
}

bool Parameter::get_bool() const {
  std::string output = boost::algorithm::to_lower_copy(value_);
  if (output == "true")
    return true;
  else if (output == "false")
    return false;
  else
    throw ImageProcessorError("Error: Casting to boolean");
}

// ParseMap
////

// adding key value pair where the key defaults to lowercase
void ParseMap::add(std::string const key, std::string const value) {
  arguments_.insert(std::pair<std::string, Parameter>(boost::to_lower_copy(key),
                                                      Parameter{value}));
}

// remove key value pair by key.
void ParseMap::remove(std::string const key) {
  arguments_.erase(key);
}

// Find returns both the key and value as a pair
std::pair<std::string, Parameter> ParseMap::find(std::string const key) const {
  auto output = arguments_.find(key);
  if (output != arguments_.end())
    return std::pair<std::string, Parameter>(output->first, output->second);
  else
    throw std::out_of_range("Key not found");
}

// Gathers keys in a vector with the option of converting these keys to
// lowercase, uppercase or as written in the yaml file
std::vector<std::string>
ParseMap::get_keys(KEYS_CASE const keys_case = KEYS_CASE::E_LOWERCASE) const {
  std::vector<std::string> key_vec;
  for (auto &pair : arguments_) {
    if (keys_case == KEYS_CASE::E_UPPERCASE)
      key_vec.push_back(boost::to_upper_copy(pair.first));
    else
      key_vec.push_back(pair.first);
  }
  return key_vec;
}

} // namespace ImageProcessor
