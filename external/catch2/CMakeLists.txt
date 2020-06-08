message(STATUS "Configuring catch2...")

add_library(catch2 INTERFACE)
target_include_directories(catch2 INTERFACE include)
target_sources(catch2 INTERFACE include/catch2/catch.h)
