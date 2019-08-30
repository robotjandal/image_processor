#ifndef CMAKE_IMAGEACTION_H
#define CMAKE_IMAGEACTION_H

#include <map>
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
  ImageAction(IMAGE_ACTIONS action, std::string str)
      : action_{action}, action_string_{str} {};
  ImageAction(IMAGE_ACTIONS action,
              std::map<std::string, std::string> arguments)
      : action_{action}, arguments_{arguments} {};
  void process(cv::Mat &image);
  std::string get_action() { return action_string_; };

private:
  const IMAGE_ACTIONS action_;
  const std::string action_string_;
  std::map<std::string, std::string> arguments_;
  // std::string input_file_;
};

class ActionFactory {
public:
  ActionFactory(std::string action) : string_action_{action} {};
  std::unique_ptr<ImageAction> generate_action();
  std::unique_ptr<ImageAction>
  generate_action(std::map<std::string, std::string> arguments);

private:
  IMAGE_ACTIONS action_;
  const std::string string_action_;

  void find_action_type();
};

} // namespace ImageProcessor
#endif