#include <iostream>

#include "parse_yaml.hpp"

namespace ImageProcessor {

// Parse Yaml
////

// Load yaml file and extract actions
std::vector<std::unique_ptr<ImageAction>> ParseYaml::parse() {
  // I believe config variable only exists when file is open.
  YAML::Node config = YAML::LoadFile(file_path_);
  // parse initialise action
  this->parse_initial(config);
  // parse remaining actions
  this->parse_actions(config);
  return std::move(actions_list_);
}

// Parse yaml variables to initialise the system
void ParseYaml::parse_initial(YAML::Node &config) {
  std::map<std::string, std::string> parameters;
  std::string image_file, output_folder;
  parameters["image"] = config["image"].as<std::string>();
  parameters["output"] = config["output"].as<std::string>();
  ActionFactory action("initialise");
  actions_list_.push_back(std::move(action.generate_action(parameters)));
  return;
}

// Use ActionFactory to create actions
void ParseYaml::parse_actions(YAML::Node &config) {
  const YAML::Node &node_actions = config["actions"];
  for (YAML::const_iterator it = node_actions.begin(); it != node_actions.end();
       it++) {
    const YAML::Node &sequence = *it;
    ActionFactory action(sequence.as<std::string>());
    actions_list_.push_back(std::move(action.generate_action()));
  }
  return;
}
} // namespace ImageProcessor
