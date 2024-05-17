add_executable(gui_demo "main.cpp")
target_link_libraries(gui_demo yage_core yage_gui)

add_custom_command(TARGET gui_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets $<TARGET_FILE_DIR:gui_demo>/assets)
