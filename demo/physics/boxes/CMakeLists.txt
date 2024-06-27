add_executable(physics_boxes
        main.cpp
        BoxApp.h
        BoxApp.cpp
        AppListener.h
        AppListener.cpp
)

target_link_libraries(physics_boxes
        PUBLIC yage_runtime
)
