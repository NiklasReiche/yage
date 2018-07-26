###################################################################
# source/modules/algo                                       #
###################################################################

include( ${YAGE_CMAKE_PATH}/sources-modules.cmake )

add_library(YAGEalgo STATIC ${YAGE_ALGO_SOURCE})

target_link_libraries(YAGEalgo PUBLIC YAGEcore
						)
                              
target_include_directories(YAGEalgo PUBLIC ${YAGE_MODULES_PATH})