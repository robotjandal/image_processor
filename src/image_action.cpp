#include "image_action.hpp"

#include <iostream>

using namespace std;

void initialise();
void save();
void histogram();
void test();

// Image Action
////

// Process action based on IMAGE_ACTIONS action
void ImageAction::process() {
  cout << "Processing Action: ";

  switch (action_) {
  case IMAGE_ACTIONS::E_INITIALISE:
    initialise();
    break;
  case IMAGE_ACTIONS::E_SAVE:
    save();
    break;
  case IMAGE_ACTIONS::E_HISTOGRAM:
    histogram();
    break;
  case IMAGE_ACTIONS::E_TEST:
    test();
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

// Other Functions
////

// Set up steps
void initialise() {
  cout << "Initial setup before processing image actions" << endl;
}

// Save image to file
void save() { cout << "Saving image" << endl; }

// Create histogram
void histogram() { cout << "saving histogram" << endl; }

// A test function to be deleted or repurposed
void test() { cout << "This is a test" << endl; }