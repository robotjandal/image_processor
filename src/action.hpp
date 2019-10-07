#ifndef CMAKE_ACTION_H
#define CMAKE_ACTION_H

#include <boost/filesystem.hpp>
#include <string>

#include "opencv4/opencv2/opencv.hpp"

namespace ImageProcessor {

struct Image {
  Image(){};
  Image(cv::Mat image, boost::filesystem::path filename, int copy = 1)
      : image_{image}, filename_{filename}, copy_{copy} {};
  cv::Mat image_;
  std::string get_filename();
  std::string output_folder_;
  Image copy();
  static void clear();

private:
  boost::filesystem::path filename_;
  int copy_{1};
  // TODO: replace with a better system
  static int max_copy_;
};

} // namespace ImageProcessor
#endif