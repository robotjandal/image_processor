#ifndef CMAKE_PARSEYAML_H
#define CMAKE_PARSEYAML_H

#include "action.hpp"
#include "parse.hpp"
#include <yaml-cpp/yaml.h>

namespace ImageProcessor {

class YamlParser {
public:
  YamlParser(std::string const input_file) : file_path_{input_file} {};
  std::vector<std::unique_ptr<Action>> parse() const;

private:
  std::string const file_path_;
};

class YamlNode {
public:
  YamlNode(){};
  YamlNode(YAML::Node const node) : node_{node} {};
  void process();
  ParseMap get_parameters() const { return parameters_; };

  bool has_found_actions() const { return this->found_action_; };
  // returns constant pointer to nested action Yaml::Node
  YAML::Node get_action_node() const { return action_node_; };

private:
  YAML::Node const node_;
  ParseMap parameters_;
  // set to true when "actions" key found in Node
  static bool found_action_;
  YAML::Node action_node_;
};

} // namespace ImageProcessor

#endif // CMAKE_PARSEYAML_H
