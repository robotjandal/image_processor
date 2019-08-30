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
  std::vector<std::unique_ptr<ImageAction>> actions_list_;
};

class YamlNode {
public:
  YamlNode(const YAML::Node node) : node_{node} {};
  std::unique_ptr<ImageAction> create_action();
  void set_action_processed() { this->action_processed_ = true; };
  // returns true if nested actions pointer was found
  // currently set to only allow one set of actions per yaml file
  bool has_unprocessed_actions();
  // returns constant pointer to nested action Yaml::Node
  const YAML::Node get_action_node() { return action_node_; };

private:
  const YAML::Node node_;
  // set to true when "actions" key found in Node
  static bool found_action_;
  // Explicitly prevents nested actions
  static bool action_processed_;
  static bool initialised_;
  YAML::Node action_node_;
};

} // namespace ImageProcessor

#endif // CMAKE_PARSEYAML_H
