#ifndef CMAKE_IMAGEACTION_H
#define CMAKE_IMAGEACTION_H

#include <string>

#include "opencv4/opencv2/opencv.hpp"

namespace ImageProcessor {

enum class IMAGE_ACTIONS : unsigned char {
  E_NOACTION = 0x00,
  E_INITIALISE = 0x01,
  E_SAVE = 0x02,
  E_CONVERTGREY = 0x03,
};

class ImageAction {
public:
  ImageAction(IMAGE_ACTIONS action) : action_{action} {};
  ImageAction(IMAGE_ACTIONS action, std::string input, std::string output)
      : action_{action}, input_file_{input}, output_folder_{output} {};
  void process(cv::Mat &image);

private:
  IMAGE_ACTIONS action_;
  std::string input_file_;
  std::string output_folder_ = "output";
};

class ActionFactory {
public:
  ActionFactory(std::string action) : string_action_{action} {};
  ImageAction *generate_action();
  ImageAction *generate_action(std::string option1, std::string option2);

private:
  IMAGE_ACTIONS action_;
  std::string string_action_;

  void find_action_type();
};

} // namespace ImageProcessor
#endif