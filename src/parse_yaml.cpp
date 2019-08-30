#include "parse_yaml.hpp"
#include <boost/log/trivial.hpp>

namespace ImageProcessor {

// Yaml Parse
////

// Convert yaml file into a vector of actions
std::vector<std::unique_ptr<ImageAction>> YamlParser::parse() {
  YAML::Node config = YAML::LoadFile(file_path_);
  // The set of outermost mappings in the yaml file are considered an action
  YamlNode initial_node{config};
  std::unique_ptr<ImageAction> initial = initial_node.create_action();
  if (initial->exists())
    actions_list_.push_back(std::move(initial));

  if (initial_node.has_unprocessed_actions()) {
    for (YAML::const_iterator it = initial_node.get_action_node().begin();
         it != initial_node.get_action_node().end(); it++) {
      BOOST_LOG_TRIVIAL(debug) << "Analyse sequence ";
      YamlNode action_node{*it};
      std::unique_ptr<ImageAction> action{action_node.create_action()};
      if (action->exists())
        actions_list_.push_back(std::move(action));
    }
  } else {
    // throw actionsmissingerror
    BOOST_LOG_TRIVIAL(warning) << "No actions found";
  }
  return std::move(actions_list_);
}

// YamlNode
////
bool YamlNode::found_action_ = false;
bool YamlNode::action_processed_ = false;
bool YamlNode::initialised_ = false;

// Returns true when actions sequence has not been processed.
// Designed to explicitly prevent nested actions
bool YamlNode::has_unprocessed_actions() {
  return found_action_ && !action_processed_;
};

// Builds arguments to create an action into a map which is used to
std::unique_ptr<ImageAction> YamlNode::create_action() {
  std::map<std::string, std::string> arguments;
  std::string action_string;
  switch (node_.Type()) {
  case YAML::NodeType::Scalar:
    BOOST_LOG_TRIVIAL(debug) << "Scalar : " << node_.as<std::string>();
    action_string = node_.as<std::string>();
    arguments.insert(
        std::pair<std::string, std::string>(node_.as<std::string>(), ""));
    break;
  case YAML::NodeType::Map:
    for (YAML::const_iterator it = node_.begin(); it != node_.end(); it++) {
      const std::string key = it->first.as<std::string>();
      // TODO: harden agaianst anything other than a string being used here.
      // exception catching?
      if (key == "actions") {
        BOOST_LOG_TRIVIAL(debug) << "Found actions sequence";
        found_action_ = true;
        action_node_ = it->second;
      } else {
        const std::string value = it->second.as<std::string>();
        BOOST_LOG_TRIVIAL(debug) << "Map. key: " << key << ", value: " << value;
        arguments.insert(std::pair<std::string, std::string>(key, value));
      }
      // to be removed, when ImageAction is updated.
      action_string = node_.begin()->first.as<std::string>();
    }
    break;
  case YAML::NodeType::Sequence:
    BOOST_LOG_TRIVIAL(warning) << "Sequence cannot be analysed";
  case YAML::NodeType::Null:
  case YAML::NodeType::Undefined:
    break;
  }

  // first action must be "initialise"
  if (!this->initialised_) {
    ActionFactory action("initialise");
    this->initialised_ = true;
    return action.generate_action(arguments);
  } else {
    ActionFactory action(action_string);
    return action.generate_action(arguments);
  }
}

} // namespace ImageProcessor
