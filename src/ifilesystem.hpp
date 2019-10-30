#ifndef CMAKE_INTERFACEFILESYSTEM_H
#define CMAKE_INTERFACEFILESYSTEM_H

#include <boost/filesystem.hpp>
#include <memory>
#include <opencv4/opencv2/opencv.hpp>

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
                           cv::Mat const image) = 0;
};

// Filesystem interaction class, a concrete implementation of IFilesystem.
class Filesystem : public IFilesystem {
public:
  bool path_exists(boost::filesystem::path const path) const override;
  void create_folders(boost::filesystem::path const folder) override;
  void remove_folder(boost::filesystem::path const folder) override;
  cv::Mat read_image(boost::filesystem::path const path, bool grey) override;
  bool write_image(boost::filesystem::path const path,
                   cv::Mat const image) override;
};

IFilesystem *make_filesystem();

} // namespace ImageProcessor

#endif