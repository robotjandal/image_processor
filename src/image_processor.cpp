#include "image_processor.hpp"

#include <iostream>

#include "boost/filesystem.hpp"
#include "parse_yaml.hpp"

#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/sources/severity_logger.hpp"

using namespace std;

namespace ImageProcessor {

// Image Processor
////

// Set up logging
bool ImageProcessor::initialise() {
  namespace logging = boost::log;

  logging::add_file_log("output/processor.log");

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::trace);

  return true;
}

// Generate and process a list of actions
bool ImageProcessor::run() {
  this->parse_file();
  if (this->perform_actions())
    return true;
  return false;
}

// process action from list
bool ImageProcessor::perform_actions() {
  cout << "Performing actions" << endl;
  for (auto const &action : actions_list_) {
    action->process(image_);
    // when replaced with a unique_ptr this line can go:
  }
  actions_list_.clear();
  return true;
}

// process input file and populate actions vector
void ImageProcessor::parse_file() {
  if (this->is_yaml()) {
    cout << "Parsing yaml file" << endl;
    ParseYaml yaml(file_path_);
    actions_list_ = yaml.parse();
  }
  return;
}

// tests file_path_ extension is .yaml
bool ImageProcessor::is_yaml() {
  namespace fs = boost::filesystem;
  fs::path file(file_path_);
  string extension = file.extension().string();
  cout << "Print input file extension: " << extension << endl;
  return (extension == string(".yaml"));
}

} // namespace ImageProcessor