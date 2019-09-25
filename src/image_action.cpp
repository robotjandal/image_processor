#include "image_action.hpp"

#include <boost/filesystem.hpp>
#include <boost/log/trivial.hpp>
#include <memory>

namespace ImageProcessor {

// void initialise(cv::Mat &image, string input_file, string output_folder);
void initialise(cv::Mat &image, std::map<std::string, std::string> arguments);
void save(cv::Mat &image);
void grey(cv::Mat &image);

// Image Action
////

// band-aid exists function, returns true when arguments exist
bool ImageAction::exists() {
  if (arguments_.size())
    return true;
  else
    return false;
};

// Process action based on IMAGE_ACTIONS action
void ImageAction::process(cv::Mat &image) {
  BOOST_LOG_TRIVIAL(debug) << "Processing Action: ";

  switch (action_) {
  case IMAGE_ACTIONS::E_INITIALISE:
    initialise(image, arguments_);
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
std::unique_ptr<ImageAction> ActionFactory::generate_action() {
  find_action_type();
  std::unique_ptr<ImageAction> action_pointer{
      new ImageAction(action_, string_action_)};
  return action_pointer;
}

// Create action object passing in additional arguments
// TODO: if the construction of the action fails it should return nullptr.
std::unique_ptr<ImageAction>
ActionFactory::generate_action(std::map<std::string, std::string> arguments) {
  find_action_type();
  std::unique_ptr<ImageAction> action_pointer(
      new ImageAction(action_, arguments));
  // ImageAction *action_object = new ImageAction(action_, arguments);
  return action_pointer;
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

// convert string to int
int Parameter::get_int() const {
  std::stringstream ss;
  int output;
  ss << value_;
  ss >> output;
  if (ss.fail())
    throw ImageProcessorError("Error: Casting to int");
  return output;
}

// convert string to float
float Parameter::get_float() const {
  std::stringstream ss;
  float output;
  ss << value_;
  ss >> output;
  if (ss.fail())
    throw ImageProcessorError("Error: Casting to float");
  return output;
}

// convert string to bool
bool Parameter::get_bool() const {
  std::string output = boost::algorithm::to_lower_copy(value_);
  if (output == "true")
    return true;
  else if (output == "false")
    return false;
  else
    throw ImageProcessorError("Error: Casting to boolean");
}

// Other Functions
////

// Set up steps for input file and output folder
// * input file is set
// * output folder is set
void initialise(cv::Mat &image, std::map<std::string, std::string> arguments) {
  std::string file = arguments["image"];
  std::string output_folder = arguments["output"];
  BOOST_LOG_TRIVIAL(debug) << "Initial setup before processing image actions";
  BOOST_LOG_TRIVIAL(debug) << "File path: " << file
                           << ". Output folder: " << output_folder;

  if (!boost::filesystem::exists(output_folder)) {
    boost::filesystem::remove_all(output_folder);
    boost::filesystem::create_directories(output_folder);
  }

  image = cv::imread(file, cv::IMREAD_COLOR);
  return;
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