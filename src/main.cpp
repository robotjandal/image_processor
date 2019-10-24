//    Analyse and manipulate an image using OpenCV by processing
//    a list of actions described in a yaml file.
//    Created by Dilpesh Patel on 2019/08/02

#include <iostream>
#include <string>

#include "image_processor.hpp"

static const char *const USAGE = "usage: ./play_image <yaml_path>\n";
static const std::string OUTPUT_FOLDER = "output";

int main(int argc, const char *argv[]) {
  // one parameter must be supplied
  bool is_ready;
  if (argc != 2) {
    std::cout << USAGE;
    return -1;
  }
  // TODO: add missing try/catch block
  std::string file_path = argv[1];
  ImageProcessor::ImageProcessor ip(file_path);
  ip.init();
  ip.run();
  std::cout << "Run Successful" << std::endl;
  return 0;
}
