#ifndef CMAKE_ACTION_H
#define CMAKE_ACTION_H

#include <string>

#include "parse.hpp"
#include <boost/filesystem.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace ImageProcessor {

struct Image {
  Image(){};
  Image(const cv::Mat image, const std::string filename,
        const std::string output_folder)
      : image_{image}, filename_{boost::filesystem::path{filename}},
        output_folder_{output_folder} {};
  Image(const cv::Mat image, const boost::filesystem::path filename,
        const std::string output_folder)
      : image_{image}, filename_{filename}, output_folder_{output_folder} {};

  const std::string get_filename() const {
    return filename_.filename().string();
  };
  const std::string get_stem() const { return filename_.stem().string(); };
  const std::string get_extension() const {
    return filename_.extension().string();
  };

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
  Initialise(const std::string input_file, const std::string output_folder)
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
  Save(const std::string filename) : filename_{filename} {};

  Image process(Image);
  static void reset() { counter_ = 0; };

private:
  std::string get_save_filename(const boost::filesystem::path filename,
                                const std::string output_folder);

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