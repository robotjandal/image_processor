#include "parse_yaml.hpp"

#include <iostream>

using namespace std;

// Parse Yaml
////

// Load yaml file and extract actions
void ParseYaml::parse() {
  // I believe config variable only exists when file is open.
  YAML::Node config = YAML::LoadFile(file_path_);
  // parse initialise action
  this->parse_initial(config);
  // parse remaining actions
  this->parse_actions(config);
}

// Parse yaml variables to initialise the system
void ParseYaml::parse_initial(YAML::Node &config) {
  string image_file, output_folder;
  image_file = config["image"].as<string>();
  output_folder = config["output"].as<string>();
  ActionFactory action("initialise");
  actions_list_->push_back(action.generate_action(image_file, output_folder));
  return;
}

// Use ActionFactory to create actions
void ParseYaml::parse_actions(YAML::Node &config) {
  const YAML::Node &node_actions = config["actions"];
  for (YAML::const_iterator it = node_actions.begin(); it != node_actions.end();
       it++) {
    const YAML::Node &sequence = *it;
    ActionFactory action(sequence.as<string>());
    actions_list_->push_back(action.generate_action());
  }
  return;
}
