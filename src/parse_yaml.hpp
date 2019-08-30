#ifndef CMAKE_PARSEYAML_H
#define CMAKE_PARSEYAML_H

#include "image_action.hpp"
#include <yaml-cpp/yaml.h>

namespace ImageProcessor {

class ParseYaml {
public:
  ParseYaml(std::string input_file) : file_path_{input_file} {};
  std::vector<std::unique_ptr<ImageAction>> parse();

private:
  const std::string file_path_;
  std::vector<std::unique_ptr<ImageAction>> actions_list_;

  void parse_initial(YAML::Node &config);
  void parse_actions(YAML::Node &config);
};
} // namespace ImageProcessor

#endif // CMAKE_PARSEYAML_H
