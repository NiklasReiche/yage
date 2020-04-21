###################################################################
# source/modules/graphics3d                                       #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )

add_library(YAGEgraphics3d STATIC ${YAGE_GRAPHICS_3D_SOURCE})

target_link_libraries(YAGEgraphics3d PUBLIC YAGEcore
								     PUBLIC YAGEimage)
                              
target_include_directories(YAGEgraphics3d PUBLIC ${YAGE_MODULES_PATH})