#include "action_factory.hpp"

#include <vector>

#include "exceptions.hpp"
#include "di_interfaces.hpp"
#include <boost/algorithm/string.hpp>

namespace ImageProcessor {

// Creates action based on supplied parameters
std::unique_ptr<Action>
ActionFactory::create_action(ParseMap const parameters) {
  std::string action = find_action(parameters);
  if (action == "save") {
    if (parameters["save"].get_string() == "")
      return std::unique_ptr<Action>{new Save{make_filesystem()}};
    return std::unique_ptr<Action>{
        new Save{make_filesystem(), parameters["save"].get_string()}};

  } else if (action == "grey")
    return std::unique_ptr<Action>{new Grey{make_opencv()}};
  else
    // a double check to ensure action was found. Nothing should reach here
    throw ImageProcessorError("No action found");
}

std::unique_ptr<Action>
ActionFactory::create_initial_action(ParseMap const parameters) {
  if (parameters["output"].get_string() == "")
    // pass in only image
    return std::unique_ptr<Action>{
        new Initialise(make_filesystem(), parameters["image"].get_string())};
  // pass in image and output folder string
  return std::unique_ptr<Action>{
      new Initialise(make_filesystem(), parameters["image"].get_string(),
                     parameters["output"].get_string())};
}

// // iterates through key's to find one of the allowed action strings.
// // If zero or more than one action is found an error is returned.
// Take's the list of Parameter key's as input and return's a string output
std::string ActionFactory::find_action(ParseMap const parameters) const {
  // actions_list must be in alphabetical order, and lowercase
  std::vector<std::string> actions_list{"grey", "save"};
  std::vector<std::string> param_vec{
      parameters.get_keys(KEYS_CASE::E_LOWERCASE)};

  // set_intersection requires sorted lists to compare
  std::sort(param_vec.begin(), param_vec.end());
  std::sort(actions_list.begin(), actions_list.end());

  // compare parameter keys and actions list and populate actions_found_vector
  // with the union
  std::vector<std::string> actions_found_vector(2);
  std::vector<std::string>::iterator actions_found_it = std::set_intersection(
      actions_list.begin(), actions_list.end(), param_vec.begin(),
      param_vec.end(), actions_found_vector.begin());
  actions_found_vector.resize(actions_found_it - actions_found_vector.begin());

  if (actions_found_vector.size() == 0)
    throw ImageProcessorError("No action found");
  else if (actions_found_vector.size() > 1)
    throw ImageProcessorError("Too many actions found");
  else {
    return actions_found_vector[0];
  }
}

} // namespace ImageProcessor