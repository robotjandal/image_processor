#include <stdexcept>

namespace ImageProcessor {

class ImageProcessorError : public std::runtime_error {
public:
  ImageProcessorError(const char *what) : runtime_error(what) {}
};

} // namespace ImageProcessor