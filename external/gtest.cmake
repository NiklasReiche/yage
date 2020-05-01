###################################################################
# GTEST	                                                          #
###################################################################

option(BUILD_GTEST "Builds the googletest subproject" ON)
#Note that googlemock target already builds googletest
option(BUILD_GMOCK "Builds the googlemock subproject" ON)

option(
  gtest_force_shared_crt
  "Use shared (DLL) run-time lib even when Google Test is built as static lib."
  ON)

add_subdirectory( ${YAGE_EXTERNAL_PATH}/googletest )

target_compile_options(gtest
        PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -w>
        $<$<CXX_COMPILER_ID:MSVC>:
        >)
target_compile_options(gmock
        PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:
        -w>
        $<$<CXX_COMPILER_ID:MSVC>:
        >)