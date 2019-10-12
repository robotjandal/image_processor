#ifndef CMAKE_ACTION_H
#define CMAKE_ACTION_H

#include <string>

#include "parse.hpp"
#include <boost/filesystem.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace ImageProcessor {

struct Image {
  Image(){};
  Image(cv::Mat const image, std::string const filename,
        std::string const output_folder)
      : image_{image}, filename_{boost::filesystem::path{filename}},
        output_folder_{output_folder} {};
  Image(cv::Mat const image, boost::filesystem::path const filename,
        std::string const output_folder)
      : image_{image}, filename_{filename}, output_folder_{output_folder} {};

  std::string get_filename() const { return filename_.filename().string(); };
  std::string get_stem() const { return filename_.stem().string(); };
  std::string get_extension() const { return filename_.extension().string(); };

  cv::Mat image_;
  boost::filesystem::path filename_;
  std::string output_folder_{"output"};
};

class Action {
public:
  Action(){};
  virtual Image process(Image) = 0;
};

// Initialise the image by clearing and creating the output folder and
// loading the image file
class Initialise : public Action {
public:
  Initialise(std::string const input_file, std::string const output_folder)
      : input_file_{input_file}, output_folder_{output_folder} {};

  Image process(Image);

private:
  std::string input_file_{""};
  std::string output_folder_{""};
};

// Save image to file based upon supplied filename otherwise
// a filename is automatically genereated
class Save : public Action {
public:
  Save(std::string const filename) : filename_{filename} {};

  Image process(Image);
  static void reset() { counter_ = 0; };

private:
  std::string get_save_filename(boost::filesystem::path const filename,
                                std::string const output_folder);

  std::string filename_{""};
  static int counter_;
};

// converts image to greyscale
class Grey : public Action {
public:
  Image process(Image);
};

} // namespace ImageProcessor
#endif