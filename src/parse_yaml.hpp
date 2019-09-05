#ifndef CMAKE_PARSEYAML_H
#define CMAKE_PARSEYAML_H

#include "image_action.hpp"
#include <yaml-cpp/yaml.h>

namespace ImageProcessor {

class YamlParser {
public:
  YamlParser(const std::string input_file) : file_path_{input_file} {};
  std::vector<std::unique_ptr<ImageAction>> parse();

private:
  const std::string file_path_;
};

class YamlNode {
public:
  YamlNode(const YAML::Node node) : node_{node} {};
  std::string process();
  std::map<std::string, std::string> get_arguments() {return arguments_;};

  bool has_found_actions()  { return this->found_action_; };
  // returns constant pointer to nested action Yaml::Node
  const YAML::Node get_action_node() { return action_node_; };

private:
  const YAML::Node node_;
  std::map<std::string, std::string> arguments_;
  // set to true when "actions" key found in Node
  static bool found_action_;
  YAML::Node action_node_;
};

} // namespace ImageProcessor

#endif // CMAKE_PARSEYAML_H
