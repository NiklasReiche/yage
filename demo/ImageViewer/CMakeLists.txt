add_executable(image_viewer "main.cpp")
target_link_libraries(image_viewer yage_core yage_image)

add_custom_command(TARGET image_viewer POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:image_viewer>/assets)
