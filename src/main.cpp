//    Analyse and manipulate an image using OpenCV by processing
//    a list of actions described in a yaml file.
//    Created by Dilpesh Patel on 2019/08/02

#include <iostream>
#include <string>

#include "image_processor.hpp"

int main() {
  ImageProcessor ip;
  if (ip.run()) {
    std::cout << "Run Successful" << std::endl;
    return 0;
  }
  std::cout << "Run Failed" << std::endl;
  return 1;
}
