add_executable(yage_physics3d_test
        collision.cpp)

target_link_libraries(yage_physics3d_test
        PRIVATE
        yage_physics3d
        Catch2::Catch2WithMain
)

add_test(
        NAME yage_physics3d_CTest
        COMMAND yage_physics3d_test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})