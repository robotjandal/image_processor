#include "action.hpp"

namespace ImageProcessor {

int Image::max_copy_ = 1;

Image Image::copy() {
  max_copy_ += 1;
  return Image{this->image_, this->filename_, Image::max_copy_};
}

void Image::clear() { max_copy_ = 1; }

// Modifies filename when more than one copy of this struct exists.
// The modification add's "_X" where X is the copy number.
std::string Image::get_filename() {
  if (copy_ > 1)
    return filename_.stem().string() + "_" + std::to_string(copy_) +
           filename_.extension().string();
  else
    return filename_.string();
}

} // namespace ImageProcessor