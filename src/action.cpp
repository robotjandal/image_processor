#include "action.hpp"

#include <vector>

#include "exceptions.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>

namespace ImageProcessor {

Initialise::Initialise(std::string const input_file,
                       std::string const output_folder)
    : input_file_{input_file}, output_folder_{output_folder} {
  if (input_file_ == "")
    throw ImageProcessorError("Input file not specified.");
  if (output_folder_ == "")
    throw ImageProcessorError("Output folder not specified.");
}

Image Initialise::process(Image image) {
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
  populate_filename(image.filename_);
  build_path(image.output_folder_);
  if (cv::imwrite(path_.string(), image.image_))
    return image;
  throw ImageProcessorError("Writing to file failed.");
}

// if filename was not set by user it is created based on the image filename
// and an increment defined by the save number. First save has no suffix.
// Each subsequent save results in appending _X (where X is a number) to the
// filename.
void Save::populate_filename(boost::filesystem::path const image_filename) {
  // counter is incremented per save so that the filename is consistent
  // independent on whether the user enters a name or not
  counter_++;
  if (!filename_.empty())
    return;
  if (counter_ == 1) {
    filename_ = image_filename;
  } else {
    std::string file = image_filename.stem().string() + "_" + std::to_string(counter_);
    boost::filesystem::path output_file{file};
    output_file.replace_extension(image_filename.extension());
    filename_ = output_file;
  }
}

std::string Save::build_path(std::string const folder) {
  boost::filesystem::path output{folder};
  path_ /=output / filename_;
}

Image Grey::process(Image image) {
  BOOST_LOG_TRIVIAL(debug) << "Converting to greyscale";
  cv::cvtColor(image.image_, image.image_, cv::COLOR_BGR2GRAY);
  return image;
}
} // namespace ImageProcessor