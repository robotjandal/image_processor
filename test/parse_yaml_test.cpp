#include "gtest/gtest.h"

#include "exceptions.hpp"
#include "parse_yaml.hpp"
#include <yaml-cpp/yaml.h>

namespace ImageProcessor {
namespace {

// Global variables
boost::filesystem::path basic_yaml = "data/all_actions.yaml";

// YamlNode testing
TEST(YamlNodeTest, scalar) {
  YAML::Node yaml;
  yaml = "Tester"; // setting mock yaml node to a Scalar value
  YamlNode node(yaml);
  node.process();
  ParseMap scalar = node.get_parameters();
  EXPECT_EQ(scalar.size(), 1);
  // Tests Scalar value has been entered
  // and that is has been set to lowercase values
  EXPECT_EQ(scalar["tester"].get_string(), "");
}

TEST(YamlNodeTest, map) {
  YAML::Node yaml;
  yaml["one"] = "Tester"; // setting mock yaml node to a map
  yaml["two"] = "2";
  yaml["THREE"] = "FOUR";

  YamlNode node(yaml);
  node.process();
  ParseMap scalar = node.get_parameters();
  EXPECT_EQ(scalar.size(), 3);
  // Tests Scalar value has been entered
  // and that is has been set to lowercase values
  EXPECT_EQ(node.has_found_actions(), false);
  EXPECT_EQ(scalar["three"].get_string(), "FOUR");
}

TEST(YamlNodeTest, sequence) {
  YAML::Node yaml;
  yaml["seq"].push_back("Tester"); // setting mock yaml node to a sequence
  yaml["seq"].push_back("2");
  yaml["seq"].push_back("FOUR");
  YamlNode node(yaml["seq"]);
  EXPECT_THROW(node.process(), ImageProcessorError);
}

TEST(YamlNodeTest, null) {
  YAML::Node yaml;
  YamlNode node(yaml);
  ASSERT_THROW(node.process(), ImageProcessorError);
}

TEST(YamlNodeTest, action_found) {
  boost::filesystem::path path = boost::filesystem::current_path();
  path /= basic_yaml;
  YAML::Node yaml = YAML::LoadFile(path.string());
  YAML::Node yaml_2 = yaml;
  YamlNode node(yaml);
  node.process();
  ParseMap map = node.get_parameters();
  EXPECT_EQ(map["output"].get_string(), "output");
  EXPECT_EQ(map.size(), 2);
  // testing actions node pointer has been set correctly
  YAML::Node action_node;
  for (YAML::const_iterator it = yaml.begin(); it != yaml.end(); it++) {
    if (it->first.as<std::string>() == "actions")
      action_node = it->second;
  }
  EXPECT_EQ(node.has_found_actions(), true);
  EXPECT_EQ(action_node, node.get_action_node());
}

// YamlParser testing

// only testing the number of actions found.
// Other tests cover more granular details
TEST(YamlParserTest, all_actions) {
  boost::filesystem::path path = boost::filesystem::current_path();
  path /= basic_yaml;
  YamlParser parser(path.string());
  std::vector<std::unique_ptr<Action>> actions = parser.parse();
  EXPECT_EQ(actions.size(), 3);
};

TEST(YamlParserTest, no_actions) {
  std::string yaml_string = "image: ./data/test_image.png \noutput: output \n";
  YAML::Node const config = YAML::Load(yaml_string);
  YamlParser parser(config);
  ASSERT_THROW(parser.parse(), ImageProcessorError);
}

} // namespace
} // namespace ImageProcessor
