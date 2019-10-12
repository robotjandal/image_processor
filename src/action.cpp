#include "action.hpp"

#include <vector>

#include "exceptions.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

namespace ImageProcessor {

Image Initialise::process(Image image) {
  if (input_file_ == "")
    throw ImageProcessorError("Input file not specified.");
  if (output_folder_ == "")
    throw ImageProcessorError("Output folder not specified.");

  BOOST_LOG_TRIVIAL(debug) << "Initial setup before processing image actions";
  BOOST_LOG_TRIVIAL(debug) << "File path: " << input_file_
                           << ". Output folder: " << output_folder_;
  if (boost::filesystem::exists(output_folder_)) {
    boost::filesystem::remove_all(output_folder_);
  }
  boost::filesystem::create_directories(output_folder_);

  image = Image{cv::imread(input_file_, cv::IMREAD_COLOR),
                boost::filesystem::path(input_file_).filename().string(),
                output_folder_};
  return image;
}

int Save::counter_ = 0;

Image Save::process(Image image) {
  BOOST_LOG_TRIVIAL(debug) << "Saving image";
  // counter is incremented per save so that the filename is consistent
  // independent on whether the user enters a name or not
  counter_++;
  if (filename_ == "")
    filename_ = image.get_filename();
  if (cv::imwrite(image.output_folder_ + "/" + filename_, image.image_))
    return image;
  throw ImageProcessorError("Writing to file failed.");
}

// converts filename to string based on number of times the output was saved
// Each subsequent save results in appending _X (where X is a number) to the
// filename.
std::string Save::get_save_filename(boost::filesystem::path const filename,
                                    std::string const output_folder) {
  if (counter_ == 1)
    return output_folder + "/" + filename.string();
  return output_folder + "/" + filename.stem().string() + "_" +
         std::to_string(counter_) + filename.extension().string();
}

Image Grey::process(Image image) {
  BOOST_LOG_TRIVIAL(debug) << "Converting to greyscale";
  cv::cvtColor(image.image_, image.image_, cv::COLOR_BGR2GRAY);
  return image;
}

} // namespace ImageProcessor