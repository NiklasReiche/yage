add_executable(simple_window "main.cpp")
target_link_libraries(simple_window yage_core)

add_custom_command(TARGET simple_window POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:simple_window>/assets)
