add_executable(model_viewer
        main.cpp
        MovementListener.h
        MovementListener.cpp
        ModelViewerApp.h
        ModelViewerApp.cpp
)

target_link_libraries(model_viewer
        PUBLIC yage_runtime
)
