#include <boost/filesystem.hpp>
#include <iostream>

#include "action.hpp"
#include "di_interfaces.hpp"
#include "di_interfaces_mock.hpp"
#include "exceptions.hpp"
#include "image.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace ImageProcessor {

using ::testing::_; // Matcher for parameters
using testing::AtLeast;
using ::testing::Return;

// Global variables
boost::filesystem::path TESTIMAGE = "./data/test_image.png";

class ImageTest : public ::testing::Test {
protected:
  void SetUp() override {
    // temporary output to screen for debugging purposes
    // remove when ./bin/unit_tests works
    std::cout << "current folder: " << boost::filesystem::current_path()
              << std::endl;
    im1_ = Image{cv::imread(TESTIMAGE.string(), cv::IMREAD_COLOR),
                 TESTIMAGE.filename().string(), "test_output"};
  }

  Image im1_{};
  Image im2_{};
};

// loading all test images work (currently only one image)
TEST_F(ImageTest, loadFile) {
  ASSERT_TRUE(boost::filesystem::exists(TESTIMAGE));
  EXPECT_EQ(im1_.image_.rows, 10);
  EXPECT_EQ(im1_.image_.cols, 10);
}

class InitialiseTest : public ::testing::Test {
protected:
  void SetUp() override {
    im1_ = Image{cv::imread(TESTIMAGE.string(), cv::IMREAD_COLOR),
                 TESTIMAGE.filename().string(), "output"};
  }
  Image im1_;
};

// test constructing the initial object using different parameters
TEST_F(InitialiseTest, InitialiseConstruction) {
  // test empty parameter use cases
  EXPECT_THROW(Initialise(make_filesystem(), ""), ImageProcessorError);
  EXPECT_THROW(Initialise(make_filesystem(), "", ""), ImageProcessorError);
  EXPECT_THROW(Initialise(make_filesystem(), "test", ""), ImageProcessorError);
  EXPECT_THROW(Initialise(make_filesystem(), "", "other"), ImageProcessorError);
  // one parameter initialisation
  Initialise one{make_filesystem(), TESTIMAGE.string()};
  EXPECT_EQ(one.get_input_file(), TESTIMAGE.string());
  EXPECT_EQ(one.get_output_folder(), "output");
  // two parameter initialisation
  Initialise two{make_filesystem(), TESTIMAGE.string(), "different"};
  EXPECT_EQ(two.get_input_file(), TESTIMAGE.string());
  EXPECT_EQ(two.get_output_folder(), "different");
}

// test processing can
// raw pointers for mockFilesystem is used because unique_ptr was too difficult
TEST_F(InitialiseTest, InitialiseProcess) {
  mockFilesystem *fs_ptr = new mockFilesystem;
  Initialise test(fs_ptr, TESTIMAGE.string());
  EXPECT_CALL(*fs_ptr, path_exists(boost::filesystem::path{"output"}))
      .WillOnce(Return(true));
  EXPECT_CALL(*fs_ptr, remove_folder(boost::filesystem::path{"output"}))
      .Times(1);
  EXPECT_CALL(*fs_ptr, create_folders(boost::filesystem::path{"output"}))
      .Times(1);
  EXPECT_CALL(*fs_ptr, read_image(TESTIMAGE, false))
      .WillOnce(Return(im1_.image_));

  Image output = test.process(im1_);
  ASSERT_EQ(output, im1_);
}

// reads in original colour image which is then transformed.
// compared with grey image already in grey.
// could not get a single channel read in through imread
class GreyTest : public ::testing::Test {
protected:
  void SetUp() override {
    im1_ = Image{cv::imread(TESTIMAGE.string()), TESTIMAGE.filename().string(),
                 "output"};
  }
  Image im1_;
};

TEST_F(GreyTest, process) {
  cv::Mat temp;
  cv::cvtColor(im1_.image_, temp, cv::COLOR_BGR2GRAY);
  mockOpenCV *cv_ptr = new mockOpenCV;
  Grey action(cv_ptr);
  Image grey_im = Image(temp, im1_.filename_.string(), im1_.output_folder_);
  EXPECT_CALL(*cv_ptr, grey(grey_im)).WillOnce(Return(grey_im));
  Image processed = action.process(grey_im);
  ASSERT_EQ(processed, grey_im);
}

// // Making multiple copies of the image and testing file path return
//}
// TEST_F(ImageTest, multiCopyReturnFilepath) {
//   Image im2 = im1.copy();
//   Image im3 = im2.copy();
//   Image im4 = im1.copy();
//   Image::clear();
//   Image im5{cv::imread(input_file.string()), input_file.filename()};
//   Image im6 = im5.copy();
//   // copy fom im1
//   Image im7 = im1.copy();
//   // copy from im5
//   Image im8 = im5.copy();

//   EXPECT_EQ(im1.get_filename(), "test_image.png");
//   EXPECT_EQ(im2.get_filename(), "test_image_2.png");
//   EXPECT_EQ(im3.get_filename(), "test_image_3.png");
//   EXPECT_EQ(im4.get_filename(), "test_image_4.png");
//   // reset back to 1
//   EXPECT_EQ(im5.get_filename(), "test_image.png");
//   EXPECT_EQ(im6.get_filename(), "test_image_2.png");
//   // currently numeric value continues up without consideration
//   EXPECT_EQ(im7.get_filename(), "test_image_3.png");
//   EXPECT_EQ(im8.get_filename(), "test_image_4.png");
// }

} // namespace ImageProcessor
