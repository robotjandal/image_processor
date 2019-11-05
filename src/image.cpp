#include "image.hpp"

namespace ImageProcessor {

bool Image::operator==(const Image &other) const {
  cv::Mat dst;
  cv::bitwise_xor(image_, other.image_, dst);
  if (filename_ == other.filename_ && output_folder_ == other.output_folder_)
    if (cv::countNonZero(dst) > 0) { // check non-0 pixels
    } else                           // false when images are equal
      return true;
  return false;
}

}