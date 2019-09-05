cmake_minimum_required(VERSION 3.9...3.10)
project(IMAGE_PROCESSOR 
    DESCRIPTION "Yaml based tool for analysing and manipulating images"
    LANGUAGES CXX
)

set(IMAGE_PROCESSOR_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(IMAGE_PROCESSOR_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(IMAGE_PROCESSOR_VERSION ${IMAGE_PROCESSOR_VERSION_MAJOR}.${IMAGE_PROCESSOR_VERSION_MINOR} CACHE STRING "version" FORCE)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# find packages
find_package(yaml-cpp REQUIRED)
find_package( Boost 
    REQUIRED
    COMPONENTS
        system
        filesystem
        log
)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


add_subdirectory(src)

# allow to report to a cdash dashboard
include(CTest)


# build tests (targets: gtest_main, gtest)
# add_subdirectory(vendor/google/googletest/googletest)

# googletest
enable_testing()
add_subdirectory(test)
# target_link_libraries("${PROJECT_NAME}-test" gtest)