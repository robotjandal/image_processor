#ifndef CMAKE_INTERFACEFILESYSTEM_H
#define CMAKE_INTERFACEFILESYSTEM_H

#include <boost/filesystem.hpp>
#include <memory>
#include <opencv4/opencv2/opencv.hpp>

#include "image.hpp"

namespace ImageProcessor {

// Interface for Filesystem access applying Dependency Injection
class IFilesystem {
public:
  virtual ~IFilesystem(){};
  virtual bool path_exists(boost::filesystem::path const path) const = 0;
  virtual void remove_folder(boost::filesystem::path const folder) = 0;
  virtual void create_folders(boost::filesystem::path const folder) = 0;
  virtual cv::Mat read_image(boost::filesystem::path const path, bool grey) = 0;
  virtual bool write_image(boost::filesystem::path const path,
                           Image const image) = 0;
};

// Filesystem interaction class, a concrete implementation of IFilesystem.
// Includes reading and writing OpenCV Mat images
class Filesystem : public IFilesystem {
public:
  bool path_exists(boost::filesystem::path const path) const override;
  void create_folders(boost::filesystem::path const folder) override;
  void remove_folder(boost::filesystem::path const folder) override;
  cv::Mat read_image(boost::filesystem::path const path, bool grey) override;
  bool write_image(boost::filesystem::path const path,
                   Image const image) override;
};

// Interface for OpenCV processes applying Dependency Injection
class IOpenCV {
public:
  virtual ~IOpenCV(){};
  virtual Image grey(Image const &image) = 0;
};

// OpenCV functions to modify images and generate plots
class OpenCV : public IOpenCV {
public:
  Image grey(Image const &image) override;
};

IFilesystem *make_filesystem();
IOpenCV *make_opencv();

} // namespace ImageProcessor

#endif