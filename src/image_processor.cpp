#include "image_processor.hpp"

#include <iostream>

#include "boost/filesystem.hpp"
#include "parse_yaml.hpp"

using namespace std;

// Image Processor
////

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
  for (auto action : actions_list_) {
    action->process();
    // when replaced with a unique_ptr this line can go:
    delete action;
  }
  actions_list_.clear();
}

// process input file and populate actions vector
void ImageProcessor::parse_file() {
  if (this->is_yaml()) {
    cout << "Parsing yaml file" << endl;
    ParseYaml yaml(file_path_, actions_list_);
    yaml.parse();
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
