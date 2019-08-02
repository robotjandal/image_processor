#ifndef CMAKE_IMAGEACTION_H
#define CMAKE_IMAGEACTION_H

#include <string>

using namespace std;

enum class IMAGE_ACTIONS : unsigned char {
  E_NOACTION = 0x00,
  E_INITIALISE = 0x01,
  E_TEST = 0x09,
  E_SAVE = 0x02,
  E_HISTOGRAM = 0x03,
};

class ImageAction {
public:
  ImageAction(IMAGE_ACTIONS action) : action_{action} {};
  void process();

private:
  IMAGE_ACTIONS action_;
  string string_arg_;
};

#endif