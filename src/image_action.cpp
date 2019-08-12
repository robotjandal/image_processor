#include "image_action.hpp"

#include <iostream>

#include <boost/filesystem.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;

void initialise(cv::Mat &image, string input_file, string output_folder);
void save(cv::Mat &image);
void grey(cv::Mat &image);

// Image Action
////

// Process action based on IMAGE_ACTIONS action
void ImageAction::process(cv::Mat &image) {
  cout << "Processing Action: ";

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
    cout << "Unrecognised action" << endl;
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
    cout << "Cannot understand action: " << string_action_ << endl;
  }
}

// Other Functions
////

// Set up steps for input file and output folder
// * input file is set
// * output folder is set
void initialise(cv::Mat &image, string file, string output_folder) {
  cout << "Initial setup before processing image actions" << endl;
  cout << "File path: " << file << ". Output folder: " << output_folder << endl;
  
  if(!boost::filesystem::exists(output_folder)) {
    boost::filesystem::remove_all(output_folder);
    boost::filesystem::create_directories(output_folder);
  }

  image = cv::imread(file, cv::IMREAD_COLOR);
}

// Save image to file
void save(cv::Mat &image) { 
  cout << "Saving image" << endl; 
  if (cv::imwrite("outputfile.jpg", image))
    return;
  // Future raise exception
}

// Create histogram
void grey(cv::Mat &image) {
  cv::Mat grey;
  cout << "converting to greyscale" << endl; 
  cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);
  image = grey;
}

