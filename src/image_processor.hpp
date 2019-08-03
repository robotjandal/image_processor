#ifndef CMAKE_IMAGEPROCESSOR_H
#define CMAKE_IMAGEPROCESSOR_H

#include <string>
#include <vector>

#include "image_action.hpp"

using namespace std;

class ImageProcessor {
public:
  ImageProcessor(string input_file) : file_path_{input_file} {};
  bool run();
  bool perform_actions();

private:
  vector<ImageAction *> actions_list_;
  string file_path_;

  bool is_yaml();
  void parse_file();
};

#endif // CMAKE_IMAGEPROCESSOR_H
