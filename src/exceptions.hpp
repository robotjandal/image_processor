#include <stdexcept>

namespace ImageProcessor {

class ImageProcessorError : public std::runtime_error {
public:
  ImageProcessorError(char const *what) : runtime_error(what) {}
};

} // namespace ImageProcessor