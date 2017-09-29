
add_executable(test-skyboxEditor main.cpp gui.h gui.cpp)
target_link_libraries(test-skyboxEditor YAGEcore
										YAGEgui
										YAGEgraphics3d)