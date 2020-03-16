#include <boost/filesystem.hpp>

#include "action_factory.hpp"
#include "exceptions.hpp"
#include "action.hpp"
#include <gtest/gtest.h>

namespace ImageProcessor{
namespace {

// Global variables
boost::filesystem::path TESTIMAGE = "./data/test_image.png";
boost::filesystem::path TESTIMAGE_GREY = "cimage_grey.png";

class ActionFactoryTest : public ::testing::Test
{
protected:
    ActionFactory factory_;
    ParseMap parameters_;
};

TEST_F(ActionFactoryTest, createInitialAction)
{
    std::string output = "other";
    parameters_.add("image", TESTIMAGE.string());
    parameters_.add("output", output);
    std::unique_ptr<Action> action_ptr = factory_.create_initial_action(parameters_);
    Initialise *initialise_ptr = dynamic_cast<Initialise *>(action_ptr.release());
    EXPECT_EQ(initialise_ptr->get_action(), "initialise");
    EXPECT_EQ(initialise_ptr->get_output_folder(), output);
    EXPECT_EQ(initialise_ptr->get_input_file(), TESTIMAGE.string());
}

TEST_F(ActionFactoryTest, createSaveAction)
{
    std::string output = "other";
    parameters_.add("save", output);
    std::unique_ptr<Action> action_ptr = factory_.create_action(parameters_);
    Save *initialise_ptr = dynamic_cast<Save *>(action_ptr.release());
    EXPECT_EQ(initialise_ptr->get_action(), "save");
    EXPECT_EQ(initialise_ptr->get_filename(), output);
}

TEST_F(ActionFactoryTest, createGreyAction)
{
    parameters_.add("grey", "");
    std::unique_ptr<Action> action_ptr = factory_.create_action(parameters_);
    Grey *initialise_ptr = dynamic_cast<Grey *>(action_ptr.release());
    EXPECT_EQ(initialise_ptr->get_action(), "grey");
}

TEST_F(ActionFactoryTest, ErrorWrongAction)
{
    parameters_.add("fail", "");
    EXPECT_THROW(factory_.create_action(parameters_), ImageProcessorError);
}

} // namespace
} // namespace ImageProcessor
