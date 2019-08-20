#ifndef CMAKE_PARSEYAML_H
#define CMAKE_PARSEYAML_H

#include <string>

#include "image_action.hpp"
#include <yaml-cpp/yaml.h>

namespace ImageProcessor {

class ParseYaml {
public:
  ParseYaml(std::string input_file, std::vector<ImageAction *> &actions_list)
      : file_path_{input_file}, actions_list_{&actions_list} {};
  void parse();

private:
  std::string file_path_;
  std::vector<ImageAction *> *actions_list_;

  void parse_initial(YAML::Node &config);
  void parse_actions(YAML::Node &config);
};
} // namespace ImageProcessor

#endif // CMAKE_PARSEYAML_H
