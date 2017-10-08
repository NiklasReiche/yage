
add_executable( coreTest core-math/vectorTest.cpp )

target_link_libraries( coreTest PUBLIC YAGEcore
                                PUBLIC gtest )