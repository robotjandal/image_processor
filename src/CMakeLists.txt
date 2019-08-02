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
    processor 
    action
 )

install(
    TARGETS image_process
    DESTINATION bin
)
