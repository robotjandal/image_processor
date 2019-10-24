#include "image_processor.hpp"

#include <iostream>

#include "parse_yaml.hpp"
#include <boost/filesystem.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

using namespace std;

namespace ImageProcessor {

// Image Processor
////

// Set up logging
void ImageProcessor::init() const {
  namespace logging = boost::log;

  logging::add_file_log("output/processor.log");

  logging::core::get()->set_filter(logging::trivial::severity >=
                                   logging::trivial::trace);
}

// Generate and process a list of actions
void ImageProcessor::run() {
  this->parse_file();
  this->perform_actions();
}

// process action from list
void ImageProcessor::perform_actions() {
  cout << "Performing actions" << endl;
  actions_list_[0]->process(image_);
  Save::reset();
  for (auto const &action : actions_list_) {
    image_ = action->process(image_);
  }
}

// process input file and populate actions vector
void ImageProcessor::parse_file() {
  YamlParser yaml(file_path_);
  actions_list_ = yaml.parse();
}

// tests file_path_ extension is .yaml
bool ImageProcessor::is_yaml() const {
  namespace fs = boost::filesystem;
  fs::path file(file_path_);
  string extension = file.extension().string();
  cout << "Print input file extension: " << extension << endl;
  return (extension == string(".yaml"));
}

} // namespace ImageProcessor