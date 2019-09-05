#include <boost/log/trivial.hpp>

#include "exceptions.hpp"
#include "parse_yaml.hpp"

namespace ImageProcessor {

// Yaml Parse
////

// Convert yaml file into a vector of actions
std::vector<std::unique_ptr<ImageAction>> YamlParser::parse() {
  std::vector<std::unique_ptr<ImageAction>> actions_list;
  const YAML::Node config = YAML::LoadFile(file_path_);
  // The set of outermost mappings in the yaml file are considered an action
  YamlNode initial_node{config};
  ActionFactory initial_action("initialise");
  initial_node.process();
  actions_list.push_back(
      std::move(initial_action.generate_action(initial_node.get_arguments())));
  int count = 0;
  if (initial_node.has_found_actions()) {
    for (YAML::const_iterator it = initial_node.get_action_node().begin();
         it != initial_node.get_action_node().end(); it++) {
      count++;
      BOOST_LOG_TRIVIAL(debug) << "Analyse sequence ";
      YamlNode action_node{*it};
      ActionFactory action(action_node.process());
      actions_list.push_back(
          std::move(action.generate_action(action_node.get_arguments())));
    }
  }
  if (!initial_node.has_found_actions() || count == 0) {
    BOOST_LOG_TRIVIAL(warning) << "No actions found";
  }
  if (actions_list.size() == (count + 1))
    return actions_list;
  else
    throw ImageProcessorError("Error: Parsing .yaml file");
}

// YamlNode
////
bool YamlNode::found_action_ = false;

// Gather's contents of nodes into a map of strings
std::string YamlNode::process() {
  std::string action_string;
  switch (node_.Type()) {
  case YAML::NodeType::Scalar:
    BOOST_LOG_TRIVIAL(debug) << "Scalar : " << node_.as<std::string>();
    arguments_.insert(
        std::pair<std::string, std::string>(node_.as<std::string>(), ""));
    break;
  case YAML::NodeType::Map:
    for (YAML::const_iterator it = node_.begin(); it != node_.end(); it++) {
      const std::string key = it->first.as<std::string>();
      // TODO: harden agaianst anything other than a string being used here.
      // exception catching?
      if (key == "actions" && !this->found_action_) {
        BOOST_LOG_TRIVIAL(debug) << "Found actions sequence";
        found_action_ = true;
        action_node_ = it->second;
      }
      // nested action throw's an error
      else if (key == "actions" && this->found_action_) {
        throw ImageProcessorError("Error: Nesting action not allowed.");
      } else {
        const std::string value = it->second.as<std::string>();
        BOOST_LOG_TRIVIAL(debug) << "Map. key: " << key << ", value: " << value;
        arguments_.insert(std::pair<std::string, std::string>(key, value));
      }
    }
    break;
  case YAML::NodeType::Sequence:
    BOOST_LOG_TRIVIAL(warning) << "Sequence cannot be analysed";
  case YAML::NodeType::Null:
  case YAML::NodeType::Undefined:
    throw ImageProcessorError("Error: Parsing .yaml file");
    break;
  }
  action_string = arguments_.begin()->first;
  return action_string;
}

} // namespace ImageProcessor
