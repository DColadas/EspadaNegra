cmake_minimum_required(VERSION 3.11)

find_package(Catch2 QUIET)
if(TARGET Catch2::Catch2)
  message("Catch2 found")
else()
  message("Catch2 NOT found, downloading...")

  include(FetchContent)
  FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2
    GIT_TAG master
  )

  FetchContent_GetProperties(catch2)
  if (NOT catch2_POPULATED)
    FetchContent_Populate(catch2)
    add_subdirectory(${catch2_SOURCE_DIR} ${catch2_BINARY_DIR})
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/contrib)
  endif()
endif()
include(Catch)
