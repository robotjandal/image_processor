#ifndef CMAKE_IMAGEPROCESSOR_H
#define CMAKE_IMAGEPROCESSOR_H

#include <string>
#include <vector>

#include "image_action.hpp"

using namespace std;

class ImageProcessor {
public:
  bool run();
  bool perform_actions();

private:
  vector<ImageAction *> actions_list_;
};

class ActionFactory {
public:
  ActionFactory(string action) : string_action_{action} {};
  ImageAction *generate_action();

private:
  IMAGE_ACTIONS action_;
  string string_action_;

  void find_action_type();
};

#endif // CMAKE_IMAGEPROCESSOR_H
