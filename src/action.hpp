#ifndef CMAKE_ACTION_H
#define CMAKE_ACTION_H

#include <string>

#include "ifilesystem.hpp"
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
  Initialise(IFilesystem *fs, std::string const input_file)
      : Initialise(fs, input_file, "output"){};
  Initialise(IFilesystem *fs, std::string const input_file,
             std::string const output_folder);
  ~Initialise() { delete fs_; };

  Image process(Image);
  std::string get_input_file() const { return input_file_; };
  std::string get_output_folder() const { return output_folder_; };

private:
  IFilesystem *fs_;
  std::string input_file_;
  std::string output_folder_;
  bool recreate_output_folder();
  bool read_image();
};

// Save image to file based upon supplied filename otherwise
// a filename is automatically genereated
class Save : public Action {
public:
  Save(IFilesystem *fs) : Save(std::move(fs), ""){};
  Save(IFilesystem *fs, std::string const filename);
  ~Save() { delete fs_; };

  Image process(Image);
  static void reset() { counter_ = 0; };
  std::string get_filepath() { return path_.string(); };

private:
  void process_filename(boost::filesystem::path const image_filename);
  void populate_filename(boost::filesystem::path const image_filename);
  std::string build_path(std::string const folder);

  IFilesystem *fs_;
  boost::filesystem::path filename_{};
  boost::filesystem::path path_{};
  static int counter_;
};

// converts image to greyscale
class Grey : public Action {
public:
  Image process(Image);
};

} // namespace ImageProcessor
#endif