#include "image_processor.hpp"

#include <iostream>

#include "image_action.hpp"

using namespace std;

// TEMP function for generating a list of actions
void list_temp(vector<ImageAction *> &vec_ptr);

// Generate and process a list of actions
bool ImageProcessor::run() {
  list_temp(actions_list_);

  if (this->perform_actions())
    return true;

  return false;
}

// process action from list
bool ImageProcessor::perform_actions() {
  for (auto action : actions_list_) {
    action->process();
    // when replaced with a unique_ptr this line can go:
    delete action;
  }
  actions_list_.clear();
}

// Create action object
ImageAction *ActionFactory::generate_action() {
  find_action_type();
  ImageAction *action_object = new ImageAction(action_);
  return action_object;
}

// Uses object input string to determine action type
void ActionFactory::find_action_type() {
  if (string_action_ == "test")
    action_ = IMAGE_ACTIONS::E_TEST;
  else if (string_action_ == "output")
    action_ = IMAGE_ACTIONS::E_INITIALISE;
  else if (string_action_ == "save")
    action_ = IMAGE_ACTIONS::E_SAVE;
  else if (string_action_ == "histogram")
    action_ = IMAGE_ACTIONS::E_HISTOGRAM;
  else {
    action_ = IMAGE_ACTIONS::E_NOACTION;
    cout << "Cannot understand action: " << string_action_ << endl;
  }
}

// To replace with yaml parsing
// Code to test out and building a list of actions to be processed.
void list_temp(vector<ImageAction *> &vec_ptr) {
  vector<ImageAction *> list;
  ActionFactory test1("test");
  vec_ptr.push_back(test1.generate_action());
  ActionFactory test2("save");
  vec_ptr.push_back(test2.generate_action());
  ActionFactory test3("histogram");
  vec_ptr.push_back(test3.generate_action());
}
