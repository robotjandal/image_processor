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
  IFilesystem *mock = make_filesystem();
  EXPECT_THROW(Initialise(mock, ""), ImageProcessorError);
  EXPECT_THROW(Initialise(mock, "", ""), ImageProcessorError);
  EXPECT_THROW(Initialise(mock, "test", ""), ImageProcessorError);
  EXPECT_THROW(Initialise(mock, "", "other"), ImageProcessorError);
  delete mock;
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

// tests focus on process returning the correct output path
class SaveTest : public ::testing::Test {
protected:
  void SetUp() override {
    im1_ = Image(cv::imread(TESTIMAGE.string()), TESTIMAGE.filename().string(),
                 "output");
  }
  Image im1_{};
};

// testing basic Save process
TEST_F(SaveTest, SaveProcessBasicTest) {
  mockFilesystem *fs_ptr = new mockFilesystem;
  Save test(fs_ptr);
  std::string expected = "output/test_image.png";
  EXPECT_CALL(*fs_ptr, write_image(_, im1_)).WillRepeatedly(Return(true));
  Image processed = test.process(im1_);
  // Saving doesn't modify the image
  ASSERT_EQ(processed, im1_);
  ASSERT_EQ(test.get_filepath(), expected);
}

// testing file path return names
TEST_F(SaveTest, SaveProcessNonSuppliedFilename) {
  mockFilesystem *fs_ptr = new mockFilesystem;
  Save test(fs_ptr);
  std::vector<std::string> expected = {"output/test_image.png",
                                       "output/test_image_2.png",
                                       "output/test_image_3.png"};
  EXPECT_CALL(*fs_ptr, write_image(_, im1_)).WillRepeatedly(Return(true));
  for (int i = 0; i < 3; i++) {
    test.process(im1_);
    ASSERT_EQ(test.get_filepath(), expected[i]);
  }
}

// test write failure returns error
TEST_F(SaveTest, SaveProcessThrow) {
  mockFilesystem *fs_ptr = new mockFilesystem;
  Save test(fs_ptr);
  EXPECT_CALL(*fs_ptr, write_image(_, im1_)).WillOnce(Return(false));
  ASSERT_THROW(test.process(im1_), ImageProcessorError);
}

// The filename is now passed in via the constructor at different points
// testing what the output path should be when called multiple times
// including a reset
TEST_F(SaveTest, SaveProcessSuppliedFilename) {
  std::vector<std::string> expected = {
      "output/test_image.png", "output/different.png",
      "output/test_image_3.png", "output/different.png",
      "output/test_image_2.png"};

  mockFilesystem *fs_ptr = new mockFilesystem;
  // Save *test_no_filename = new Save(fs_ptr);
  Save test_no_filename = Save(fs_ptr);
  mockFilesystem *fs_ptr2 = new mockFilesystem;
  Save test_with_filename(fs_ptr2, "different");
  // EXPECT_CALL(*fs_ptr, write_image(_, im1_)).WillRepeatedly(Return(true));
  EXPECT_CALL(*fs_ptr, write_image(_, im1_)).WillRepeatedly(Return(true));
  EXPECT_CALL(*fs_ptr2, write_image(_, im1_)).WillRepeatedly(Return(true));

  for (int i = 0; i < 5; i++) {
    switch (i) {
    case 2:
    case 0:
    case 4: // not supplying Save::filename_
      test_no_filename.process(im1_);
      ASSERT_EQ(test_no_filename.get_filepath(), expected[i]);
      break;
    case 3: // reset happens before the fourth iteration
      Save::reset();
    case 1: // supplying Save::filename_
      test_with_filename.process(im1_);
      ASSERT_EQ(test_with_filename.get_filepath(), expected[i]);
      break;
    }
  }
}

} // namespace ImageProcessor
