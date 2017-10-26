add_executable(bla "main.cpp")
target_link_libraries(bla PUBLIC YAGEcore
								   PUBLIC YAGEimage
								   PUBLIC YAGEfont
								   PUBLIC YAGEgui
								   PUBLIC YAGEgraphics3d)
