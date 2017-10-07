###################################################################
# GTEST	                                                          #
###################################################################

option(BUILD_GTEST "Builds the googletest subproject" ON)
#Note that googlemock target already builds googletest
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)

add_subdirectory( ${YAGE_EXTERNAL_PATH}/googletest )