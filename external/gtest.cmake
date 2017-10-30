###################################################################
# GTEST	                                                          #
###################################################################

option(BUILD_GTEST "Builds the googletest subproject" ON)
#Note that googlemock target already builds googletest
option(BUILD_GMOCK "Builds the googlemock subproject" OFF)

option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON)

add_subdirectory( ${YAGE_EXTERNAL_PATH}/googletest )