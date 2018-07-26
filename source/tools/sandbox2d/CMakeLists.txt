
add_executable( sandbox2d "main.cpp" "app.h" )
target_link_libraries( sandbox2d PUBLIC YAGEcore
							PUBLIC YAGEgraphics2d 
							) 