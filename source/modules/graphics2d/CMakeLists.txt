###################################################################
# source/modules/graphics2d                                       #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )

add_library(YAGEgraphics2d STATIC ${YAGE_GRAPHICS_2D_SOURCE})

target_link_libraries(YAGEgraphics2d PUBLIC YAGEcore
								     PUBLIC YAGEimage)
                              
target_include_directories(YAGEgraphics2d PUBLIC ${YAGE_MODULES_PATH})