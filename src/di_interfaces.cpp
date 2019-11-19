#include "di_interfaces.hpp"
#include "exceptions.hpp"

namespace ImageProcessor {

// create filesystem raw pointer.
IFilesystem *make_filesystem() { return new Filesystem; }

// create opencv functions wrapper raw pointer
IOpenCV *make_opencv() { return new OpenCV; }


// wrapper for boost filesystem path exists
bool Filesystem::path_exists(boost::filesystem::path const folder) const {
  return boost::filesystem::exists(folder);
}

// wrapper for boost filesystem path create_directories
void Filesystem::create_folders(boost::filesystem::path const folder) {
  if (boost::filesystem::create_directories(folder))
    return;
  throw ImageProcessorError("Failed to create directories");
}

// wrapper for boost filesystem path remove_folder
void Filesystem::remove_folder(boost::filesystem::path const folder) {
  boost::filesystem::remove_all(folder);
}

// wrapper for OpenCV  imread
// defaults to opening colour version of the file
cv::Mat Filesystem::read_image(boost::filesystem::path const path,
                               bool const grey) {
  if (grey)
    return cv::imread(path.string(), cv::IMREAD_GRAYSCALE);
  return cv::imread(path.string(), cv::IMREAD_COLOR);
}

// wrapper for OpenCV  imwrite
bool Filesystem::write_image(boost::filesystem::path const path,
                             Image const image) {
  if (cv::imwrite(path.string(), image.image_))
    return true;
  return false;
}

Image OpenCV::grey(Image const &image) {
  // temporary way to create a copy of the Image object
  Image output;
  output.filename_ = image.filename_;
  output.output_folder_ = image.output_folder_;
  cv::cvtColor(image.image_, output.image_, cv::COLOR_BGR2GRAY);
  return output;
}

} // namespace ImageProcessor
