#ifndef CMAKE_IMAGEACTION_H
#define CMAKE_IMAGEACTION_H

#include <string>

using namespace std;

enum class IMAGE_ACTIONS : unsigned char {
  E_NOACTION = 0x00,
  E_INITIALISE = 0x01,
  E_SAVE = 0x02,
  E_HISTOGRAM = 0x03,
};

class ImageAction {
public:
  ImageAction(IMAGE_ACTIONS action) : action_{action} {};
  ImageAction(IMAGE_ACTIONS action, string input, string output)
      : action_{action}, input_file_{input}, output_folder_{output} {};
  void process();

private:
  IMAGE_ACTIONS action_;
  string input_file_;
  string output_folder_ = "output";
};

class ActionFactory {
public:
  ActionFactory(string action) : string_action_{action} {};
  ImageAction *generate_action();
  ImageAction *generate_action(string option1, string option2);

private:
  IMAGE_ACTIONS action_;
  string string_action_;

  void find_action_type();
};

#endif