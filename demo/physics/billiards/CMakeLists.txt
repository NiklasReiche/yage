add_executable(physics_billiards
        main.cpp
        BilliardsApp.h
        BilliardsApp.cpp
        AppListener.h
        AppListener.cpp
)

target_link_libraries(physics_billiards
        PUBLIC yage_runtime
)
