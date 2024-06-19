add_executable(model_viewer
	"main.cpp"
	"MovementListener.h"
	"app.h" 
)

target_link_libraries(model_viewer
	PUBLIC yage_core yage_gl3d
)

add_custom_command(TARGET model_viewer POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:model_viewer>/assets)
