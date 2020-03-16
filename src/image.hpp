#ifndef CMAKE_IMAGE_H
#define CMAKE_IMAGE_H

#include <string>
#include <opencv4/opencv2/opencv.hpp>
#include <boost/filesystem.hpp>

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

  bool operator==(const Image &other) const;

  bool image_exists() const;
  std::string get_filename() const { return filename_.filename().string(); };
  std::string get_stem() const { return filename_.stem().string(); };
  std::string get_extension() const { return filename_.extension().string(); };

  cv::Mat image_;
  boost::filesystem::path filename_;
  std::string output_folder_{"output"};
};

}
#endif