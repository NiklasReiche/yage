add_executable(yage_math_test
        mathTest.cpp
        vectorTest.cpp
        matrixTest.cpp
        quaternionTest.cpp)

target_link_libraries(yage_math_test
        PRIVATE yage_math Catch2::Catch2WithMain
)

add_test(
        NAME yage_math_CTest
        COMMAND yage_math_test
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
