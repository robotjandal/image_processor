# Taken from the following:
# https://github.com/google/googletest/blob/release-1.8.1/googletest/README.md
# Download and unpack googletest at configure time
configure_file(${PROJECT_SOURCE_DIR}/cmake/googletest-download.cmake googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL
)

# Now simply link against gtest or gtest_main as needed. Eg
# find_library(parse_yaml REQUIRED)

# using include_directories is not best practice. In future a better method is needed here
include_directories(${PROJECT_SOURCE_DIR}/src)

include(GoogleTest)

add_executable(unit_tests
    main.cpp
    parse_yaml_test.cpp 
    parse_test.cpp
    action_test.cpp
    ifilsystem_mock.hpp
)

target_link_libraries(unit_tests 
  PRIVATE
    gtest_main
    gmock
    interface
    action
    parse
    factory
    parse_yaml
    ${OpenCV_LIBS}
    Boost::system
    Boost::filesystem
    Boost::log
)

configure_file(data/test_image.png ${CMAKE_CURRENT_BINARY_DIR}/data/test_image.png COPYONLY)
# add_test(NAME unit_tests COMMAND unit_tests)
gtest_discover_tests(unit_tests)
# add_test(Name ParameterTest COMMAND unit_tests)
