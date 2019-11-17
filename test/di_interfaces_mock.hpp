#ifndef IM_DI_MOCKINTERFACES_H
#define IM_DI_MOCKINTERFACES_H

#include <gmock/gmock.h>

#include "di_interfaces.hpp"
#include "image.hpp"

namespace ImageProcessor {

class mockFilesystem : public IFilesystem {
public:
  mockFilesystem(){};
  virtual ~mockFilesystem(){};
  MOCK_METHOD(bool, path_exists, (boost::filesystem::path const path),
              (const, override));
  MOCK_METHOD(void, remove_folder, (boost::filesystem::path const folder),
              (override));
  MOCK_METHOD(void, create_folders, (boost::filesystem::path const folder),
              (override));
  MOCK_METHOD(cv::Mat, read_image,
              (boost::filesystem::path const path, bool grey), (override));
  MOCK_METHOD(bool, write_image,
              (boost::filesystem::path const path, Image const image),
              (override));
};

class mockOpenCV : public IOpenCV {
public:
  mockOpenCV(){};
  virtual ~mockOpenCV(){};
  MOCK_METHOD(Image, grey, (Image const &image), (override));
};

} // namespace ImageProcessor

#endif
