add_executable(text_renderer "main.cpp")
target_link_libraries(text_renderer yage_core yage_font)

add_custom_command(TARGET text_renderer POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:text_renderer>/assets)
