
add_executable(test-image main.cpp)
target_link_libraries(test-image YAGEcore
								 YAGEimage
								 zlib32s.lib
								 libpng32s.lib)