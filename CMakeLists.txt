cmake_minimum_required(VERSION 3.10)
project(IMAGE_PROCESSOR 
    DESCRIPTION "Yaml based tool for analysing and manipulating images"
    LANGUAGES CXX
)
# set(CMAKE_VERBOSE_MAKEFILE ON)
set(IMAGE_PROCESSOR_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(IMAGE_PROCESSOR_VERSION_MINOR 1 CACHE STRING "minor version" FORCE)
set(IMAGE_PROCESSOR_VERSION ${IMAGE_PROCESSOR_VERSION_MAJOR}.${IMAGE_PROCESSOR_VERSION_MINOR} CACHE STRING "version" FORCE)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# add_definitions(-DBOOST_LOG_DYN_LINK)


# find packages
find_package(yaml-cpp REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
find_package( Boost 
    REQUIRED
    COMPONENTS
        system
        filesystem
        log
)
include_directories( ${Boost_INCLUDE_DIRS})
# ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

find_package( OpenCV REQUIRED 
    core 
    imgcodecs 
    highgui
)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(src)

# enable cdash dashboard
include(CTest)
# googletest
enable_testing()
add_subdirectory(test)
