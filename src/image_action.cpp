#include "image_action.hpp"

#include <iostream>

using namespace std;

void initialise();
void save();
void histogram();
void test();

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
    break;
  }
}

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