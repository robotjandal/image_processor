#ifndef CMAKE_PARSEYAML_H
#define CMAKE_PARSEYAML_H

#include <string>

#include "image_action.hpp"
#include <yaml-cpp/yaml.h>

using namespace std;

class ParseYaml {
public:
  ParseYaml(string input_file, vector<ImageAction *> &actions_list)
      : file_path_{input_file}, actions_list_{&actions_list} {};
  void parse();

private:
  string file_path_;
  vector<ImageAction *> *actions_list_;

  void parse_actions(YAML::Node config);
};

#endif // CMAKE_PARSEYAML_H
