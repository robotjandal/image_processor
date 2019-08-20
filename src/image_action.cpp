#include "image_action.hpp"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

namespace ImageProcessor {

void initialise(cv::Mat &image, string input_file, string output_folder);
void save(cv::Mat &image);
void grey(cv::Mat &image);

// Image Action
////

// Process action based on IMAGE_ACTIONS action
void ImageAction::process(cv::Mat &image) {
  BOOST_LOG_TRIVIAL(debug) << "Processing Action: ";

  switch (action_) {
  case IMAGE_ACTIONS::E_INITIALISE:
    initialise(image, input_file_, output_folder_);
    break;
  case IMAGE_ACTIONS::E_SAVE:
    save(image);
    break;
  case IMAGE_ACTIONS::E_CONVERTGREY:
    grey(image);
    break;
  case IMAGE_ACTIONS::E_NOACTION:
  default:
    BOOST_LOG_TRIVIAL(debug) << "Unrecognised action";
    break;
  }
}

// Action Factory
////

// Create action object
ImageAction *ActionFactory::generate_action() {
  find_action_type();
  ImageAction *action_object = new ImageAction(action_);
  return action_object;
}
// Create action object passing in addtional strings
ImageAction *ActionFactory::generate_action(string option1, string option2) {
  find_action_type();
  ImageAction *action_object = new ImageAction(action_, option1, option2);
  return action_object;
}

// Uses object input string to determine action type
void ActionFactory::find_action_type() {
  if (string_action_ == "initialise")
    action_ = IMAGE_ACTIONS::E_INITIALISE;
  else if (string_action_ == "output")
    action_ = IMAGE_ACTIONS::E_INITIALISE;
  else if (string_action_ == "save")
    action_ = IMAGE_ACTIONS::E_SAVE;
  else if (string_action_ == "grey")
    action_ = IMAGE_ACTIONS::E_CONVERTGREY;
  else {
    action_ = IMAGE_ACTIONS::E_NOACTION;
    BOOST_LOG_TRIVIAL(debug) << "Cannot process action: " << string_action_;
  }
}

// Other Functions
////

// Set up steps for input file and output folder
// * input file is set
// * output folder is set
void initialise(cv::Mat &image, string file, string output_folder) {
  BOOST_LOG_TRIVIAL(debug) << "Initial setup before processing image actions";
  BOOST_LOG_TRIVIAL(debug) << "File path: " << file << ". Output folder: " << output_folder;

  if (!boost::filesystem::exists(output_folder)) {
    boost::filesystem::remove_all(output_folder);
    boost::filesystem::create_directories(output_folder);
  }

  image = cv::imread(file, cv::IMREAD_COLOR);
}

// Save image to file
void save(cv::Mat &image) {
  BOOST_LOG_TRIVIAL(debug) << "Saving image";
  if (cv::imwrite("output/output.jpg", image))
    return;
  // Future raise exception
}

// Create histogram
void grey(cv::Mat &image) {
  cv::Mat grey;
  BOOST_LOG_TRIVIAL(debug) << "converting to greyscale";
  cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);
  image = grey;
}

} // namespace ImageProcessor