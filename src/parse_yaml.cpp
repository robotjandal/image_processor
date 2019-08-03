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
  // TODO
  // parse remaining actions
  this->parse_actions(config);
}

// Use ActionFactory to create actions
void ParseYaml::parse_actions(YAML::Node config) {
  const YAML::Node &node_actions = config["actions"];
  for (YAML::const_iterator it = node_actions.begin(); it != node_actions.end();
       it++) {
    const YAML::Node &sequence = *it;
    ActionFactory action(sequence.as<string>());
    actions_list_->push_back(action.generate_action());
  }
  return;
}
