#ifndef CMAKE_IMAGEPROCESSOR_H
#define CMAKE_IMAGEPROCESSOR_H

#include <string>
#include <vector>

#include "action.hpp"
#include <opencv4/opencv2/opencv.hpp>

namespace ImageProcessor {

class ImageProcessor {
public:
  ImageProcessor(std::string const input_file) : file_path_{input_file} {};
  void init() const;
  void run();

private:
  std::vector<std::unique_ptr<Action>> actions_list_;
  const std::string file_path_;
  Image image_;

  bool is_yaml() const;
  void parse_file();
  void perform_actions();
};

} // namespace ImageProcessor

#endif // CMAKE_IMAGEPROCESSOR_H
