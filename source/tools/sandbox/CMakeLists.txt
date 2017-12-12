
add_executable( sandbox "main.cpp" "app.h" )
target_link_libraries( sandbox PUBLIC YAGEcore
							PUBLIC YAGEgraphics3d 
							) 