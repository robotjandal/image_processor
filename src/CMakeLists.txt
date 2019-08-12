find_package(yaml-cpp REQUIRED)
find_package( Boost 1.65
    REQUIRED
    COMPONENTS
        system
        filesystem
)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_library(parse_yaml
    parse_yaml.hpp
    parse_yaml.cpp
)

add_library(processor 
    image_processor.hpp
    image_processor.cpp
)

add_library(action 
    image_action.hpp
    image_action.cpp
)

add_executable(image_process
    main.cpp
)

target_link_libraries(image_process 
    PUBLIC    
        processor 
        action
        parse_yaml
    PRIVATE
        yaml-cpp
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}

 )

install(
    TARGETS image_process
    DESTINATION bin
)
