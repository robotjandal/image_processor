#include <algorithm>
#include <stdexcept>
#include <vector>

#include "exceptions.hpp"
#include "parse.hpp"
#include <gtest/gtest.h>

namespace ImageProcessor {
namespace {

// parameter::get_string()
TEST(ParameterTestString, Success) {
  std::string test = "test";
  Parameter parameter{test};
  EXPECT_EQ(parameter.get_string(), test);
}

// parameter::get_int()

TEST(ParameterTestInt, intInputSuccess) {
  std::vector<int> input_vector = {5, 1, 0, -1, -5, -10};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    std::stringstream ss;
    std::string number_str;
    ss << i;
    ss >> number_str;
    parameter_vector.push_back(Parameter{number_str});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    EXPECT_EQ(parameter_vector[i].get_int(), input_vector[i]);
  }
}

TEST(ParameterTestInt, stringInputSuccess) {
  std::vector<std::string> input_vector = {"10", "-5", "1", "3"};
  std::vector<int> int_vector = {10, -5, 1, 3};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    parameter_vector.push_back(Parameter{i});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    EXPECT_EQ(parameter_vector[i].get_int(), int(int_vector[i]));
  }
}

TEST(ParameterTestInt, stringInputError) {
  std::vector<std::string> input_vector = {"e", "-fwe", "!", "*"};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    std::stringstream ss;
    std::string number_str;
    ss << i;
    ss >> number_str;
    parameter_vector.push_back(Parameter{number_str});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    ASSERT_THROW(parameter_vector[i].get_int(), ImageProcessorError);
  }
}

TEST(ParameterTestInt, intLargeValue) {
  long int large_int = 1000000000000;
  std::stringstream ss;
  std::string number_str;
  ss << large_int;
  ss >> number_str;
  Parameter parameter{number_str};
  // implicit conversion to long int
  ASSERT_THROW(parameter.get_int(), ImageProcessorError);
}

// parameter::get_float()

TEST(ParameterTestFloat, floatInputSuccess) {
  std::vector<float> input_vector = {5, -1, 0, -1.2, 2.5, 10};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    std::stringstream ss;
    std::string number_str;
    ss << i;
    ss >> number_str;
    parameter_vector.push_back(Parameter{number_str});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    EXPECT_EQ(parameter_vector[i].get_float(), input_vector[i]);
  }
}

TEST(ParameterTestFloat, floatInputError) {
  std::vector<std::string> input_vector = {"e", "-fwe", "!", "*"};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    std::stringstream ss;
    std::string number_str;
    ss << i;
    ss >> number_str;
    parameter_vector.push_back(Parameter{number_str});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    ASSERT_THROW(parameter_vector[i].get_float(), ImageProcessorError);
  }
}

TEST(ParameterTestFloat, floatLargeValue) {
  double large_float = 100000000000000.0;
  std::stringstream ss;
  std::string number_str;
  ss << large_float;
  ss >> number_str;
  Parameter parameter{number_str};
  // implicit conversion to double
  EXPECT_FLOAT_EQ(parameter.get_float(), large_float);
}

TEST(ParameterTestBool, boolInputSuccess) {
  std::vector<std::string> input_vector = {"true",  "True",  "TRUE",
                                           "false", "False", "FALSE"};
  std::vector<bool> bool_vector = {true, true, true, false, false, false};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    parameter_vector.push_back(Parameter{i});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    EXPECT_EQ(parameter_vector[i].get_bool(), bool_vector[i]);
  }
}

TEST(ParameterTestBool, boolInputFailure) {
  std::vector<std::string> input_vector = {"0", "1", "three", "/", "+"};
  std::vector<Parameter> parameter_vector;
  for (auto &i : input_vector) {
    parameter_vector.push_back(Parameter{i});
  }
  for (int i = 0; i < input_vector.size(); i++) {
    ASSERT_THROW(parameter_vector[i].get_bool(), ImageProcessorError);
  }
}

class ParseMapTest : public ::testing::Test {
protected:
  void SetUp() override {
    p1_.add("key", "value");
    p2_.add("test1", "-1");
    p2_.add("test2", "2.2");
  }

  ParseMap p0_;
  ParseMap p1_;
  ParseMap p2_;
};

// testing adding to ParseMap
TEST_F(ParseMapTest, addKeys) {
  EXPECT_EQ(p0_.size(), 0);
  EXPECT_EQ(p1_.size(), 1);
  EXPECT_EQ(p2_.size(), 2);
  p1_.add("float", "9.6");
  EXPECT_EQ(p1_.size(), 2);
}

// testing getting values by key and casting Parameter as seen fit
TEST_F(ParseMapTest, getValues) {
  EXPECT_EQ(p1_["key"].get_string(), "value");
  EXPECT_EQ(p2_["test1"].get_int(), int(-1));
  EXPECT_EQ(p2_["test2"].get_float(), float(2.2));
  ASSERT_THROW(p1_["key"].get_int(), ImageProcessorError);
  ASSERT_THROW(p0_["not found"].get_string(), std::out_of_range);
}

// testing finding values and keys through search
TEST_F(ParseMapTest, findKeysAndValues) {
  auto output = p1_.find("key");
  EXPECT_EQ(output.first, "key");
  EXPECT_EQ(output.second.get_string(), "value");
  ASSERT_THROW(p1_["key"].get_int(), ImageProcessorError);
  ASSERT_THROW(p1_.find("not found"), std::out_of_range);
}

} // namespace
} // namespace ImageProcessor
