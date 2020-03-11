#include "parse_yaml.hpp"

#include "action_factory.hpp"
#include "exceptions.hpp"
#include <boost/log/trivial.hpp>

namespace ImageProcessor {

// Yaml Parse
////
YamlParser::YamlParser(std::string input_file) {
  yaml_ = YAML::LoadFile(input_file);
}

// Convert yaml file into a vector of actions
std::vector<std::unique_ptr<Action>> YamlParser::parse() const {
  std::vector<std::unique_ptr<Action>> actions_list;
  // YAML::Node const config = YAML::LoadFile(file_path_);
  // The set of outermost mappings in the yaml file are considered an action
  YamlNode initial_node{yaml_};
  ActionFactory factory;
  initial_node.process();
  actions_list.push_back(
      std::move(factory.create_initial_action(initial_node.get_parameters())));
  int count = 0;
  if (initial_node.has_found_actions()) {
    for (YAML::const_iterator it = initial_node.get_action_node().begin();
         it != initial_node.get_action_node().end(); it++) {
      count++;
      BOOST_LOG_TRIVIAL(debug) << "Analyse sequence ";
      YamlNode action_node{*it};
      action_node.process();
      actions_list.push_back(
          std::move(factory.create_action(action_node.get_parameters())));
    }
  }
  if (!initial_node.has_found_actions() || count == 0) {
    BOOST_LOG_TRIVIAL(warning) << "No actions found";
    throw ImageProcessorError("Error: Parsing .yaml file");
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
void YamlNode::process() {
  switch (node_.Type()) {
  case YAML::NodeType::Scalar:
    BOOST_LOG_TRIVIAL(debug) << "Scalar : " << node_.as<std::string>();
    parameters_.add(node_.as<std::string>(), "");
    break;
  case YAML::NodeType::Map:
    for (YAML::const_iterator it = node_.begin(); it != node_.end(); it++) {
      std::string const key = it->first.as<std::string>();
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
        std::string const value = it->second.as<std::string>();
        BOOST_LOG_TRIVIAL(debug) << "Map. key: " << key << ", value: " << value;
        parameters_.add(key, value);
      }
    }
    break;
  // can only handle Maps or Scalars at present
  case YAML::NodeType::Sequence:
    BOOST_LOG_TRIVIAL(warning) << "Sequence cannot be analysed";
  case YAML::NodeType::Null:
  case YAML::NodeType::Undefined:
    throw ImageProcessorError("Error: Parsing .yaml file");
    break;
  }
}

} // namespace ImageProcessor
