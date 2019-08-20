#ifndef CMAKE_IMAGEPROCESSOR_H
#define CMAKE_IMAGEPROCESSOR_H

#include <string>
#include <vector>

#include "image_action.hpp"
#include "opencv4/opencv2/opencv.hpp"

namespace ImageProcessor {

class ImageProcessor {
public:
  ImageProcessor(std::string input_file) : file_path_{input_file} {};
  bool initialise();
  bool run();
  bool perform_actions();

private:
  std::vector<ImageAction *> actions_list_;
  std::string file_path_;
  cv::Mat image_;

  bool is_yaml();
  void parse_file();
};

} // namespace ImageProcessor

#endif // CMAKE_IMAGEPROCESSOR_H
