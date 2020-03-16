#include "image.hpp"

namespace ImageProcessor {

bool Image::operator==(const Image &other) const {
  cv::Mat dst;
  cv::bitwise_xor(image_, other.image_, dst);
  if (filename_ == other.filename_ && output_folder_ == other.output_folder_ &&
      // compares equality of pixels in 1-4 channels
      sum(image_ != other.image_) == cv::Scalar(0, 0, 0, 0))
    return true;
  return false;
}

// return true if image exists otherwise return false
bool Image::image_exists() const {
  if (image_.empty())
    return false;
  return true;
}

} // namespace ImageProcessor