#ifndef CMAKE_ACTION_H
#define CMAKE_ACTION_H

#include <string>

#include "di_interfaces.hpp"
#include "image.hpp"
#include "parse.hpp"
#include <boost/filesystem.hpp>
#include <opencv4/opencv2/opencv.hpp>

namespace ImageProcessor {

class Action {
public:
  Action(){};
  virtual Image process(Image) = 0;
  std::string get_action() { return action_; };

protected:
  std::string action_;
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
  Save(IFilesystem *fs) : Save(fs, ""){};
  Save(IFilesystem *fs, std::string const filename);
  ~Save() { delete fs_; };

  Image process(Image image);
  static void reset() { counter_ = 0; };
  std::string get_filepath() { return path_.string(); };
  std::string get_filename() { return filename_.string(); };

private:
  void process_filename(boost::filesystem::path const image_filename);
  void build_custom_filename(boost::filesystem::path const image_filename);
  std::string prefix_folder(std::string const folder);

  IFilesystem *fs_;
  boost::filesystem::path filename_{}; // set by constructor
  boost::filesystem::path path_{};     // set by process()
  static int counter_;
};

// converts image to greyscale
class Grey : public Action {
public:
  Grey(IOpenCV *opencv);
  Image process(Image image);
  ~Grey() { delete cv_; };

private:
  IOpenCV *cv_;
};

} // namespace ImageProcessor
#endif