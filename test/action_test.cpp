#include <iostream>
#include <boost/filesystem.hpp>
#include <gtest/gtest.h>

#include "action.hpp"

namespace ImageProcessor {
namespace {

class ImageTest : public ::testing::Test {
protected:
  void SetUp() override {
    input_file = "./data/test_image.png";
    // temporary output to screen for debugging purposes
    // remove when ./bin/unit_tests works
    std::cout << "current folder: " << boost::filesystem::current_path()
              << std::endl;
    im1 = Image{cv::imread(input_file.string()), input_file.filename()};
  }

  Image im1{};
  boost::filesystem::path input_file;
};

// test loading a dummy image work
TEST_F(ImageTest, loadFile) {
  EXPECT_EQ(im1.image_.rows, 10);
  EXPECT_EQ(im1.image_.cols, 10);
}

// Making multiple copies of the image and testing file path return
TEST_F(ImageTest, multiCopyReturnFilepath) {
  Image im2 = im1.copy();
  Image im3 = im2.copy();
  Image im4 = im1.copy();
  Image::clear();
  Image im5{cv::imread(input_file.string()), input_file.filename()};
  Image im6 = im5.copy();
  // copy fom im1
  Image im7 = im1.copy();
  // copy from im5
  Image im8 = im5.copy();

  EXPECT_EQ(im1.get_filename(), "test_image.png");
  EXPECT_EQ(im2.get_filename(), "test_image_2.png");
  EXPECT_EQ(im3.get_filename(), "test_image_3.png");
  EXPECT_EQ(im4.get_filename(), "test_image_4.png");
  // reset back to 1
  EXPECT_EQ(im5.get_filename(), "test_image.png");
  EXPECT_EQ(im6.get_filename(), "test_image_2.png");
  // currently numeric value continues up without consideration
  EXPECT_EQ(im7.get_filename(), "test_image_3.png");
  EXPECT_EQ(im8.get_filename(), "test_image_4.png");
}

} // namespace
} // namespace ImageProcessor
