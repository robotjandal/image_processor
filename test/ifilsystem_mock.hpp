#ifndef IM_MOCKIFILSYSTEM_H
#define IM_MOCKIFILSYSTEM_H

#include "ifilesystem.hpp"
#include <gmock/gmock.h>

namespace ImageProcessor {

class mockFilesystem : public IFilesystem {
public:
  mockFilesystem() {}
  virtual ~mockFilesystem() {}
  MOCK_METHOD(bool, path_exists, (boost::filesystem::path const path),
              (const, override));
  MOCK_METHOD(void, remove_folder, (boost::filesystem::path const folder),
              (override));
  MOCK_METHOD(void, create_folders, (boost::filesystem::path const folder),
              (override));
  MOCK_METHOD(cv::Mat, read_image,
              (boost::filesystem::path const path, bool grey), (override));
  MOCK_METHOD(bool, write_image,
              (boost::filesystem::path const path, cv::Mat const image),
              (override));
};

} // namespace ImageProcessor

#endif
