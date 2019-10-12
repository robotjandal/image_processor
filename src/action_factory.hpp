#ifndef CMAKE_ACTIONFACTORY_H
#define CMAKE_ACTIONFACTORY_H

#include <memory>
#include <string>

#include "action.hpp"
#include "parse.hpp"

namespace ImageProcessor {

// Using ParseMap an action is found and the corresponding action generated.
// The returned Action is housed in a unqiue_ptr
class ActionFactory {
public:
  ActionFactory(){};
  std::unique_ptr<Action> create_action(ParseMap const parameters);
  std::unique_ptr<Action> create_initial_action(ParseMap const parameters);

private:
  std::string find_action(ParseMap const parameters) const;
  std::unique_ptr<Action> create();
  std::string name{};
};

} // namespace ImageProcessor
#endif