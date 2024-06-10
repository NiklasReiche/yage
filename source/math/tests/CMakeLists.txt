add_executable(yage_math_test
        mathTest.cpp
        vectorTest.cpp
        matrixTest.cpp
        quaternionTest.cpp)

target_link_libraries(yage_math_test
        PRIVATE yage_math Catch2::Catch2WithMain
)

target_compile_options(yage_math_test
        PRIVATE -fdiagnostics-color=always -Wall -Wextra -Werror)

add_test(
        NAME yage_gml_CTest
        COMMAND yage_gml_test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
