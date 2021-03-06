cmake_minimum_required(VERSION 3.11)

project(EspadaNegra CXX)
include(cmake/StandardProjectSettings.cmake)
add_library(project_options INTERFACE)

# Set used C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable cache
include(cmake/Cache.cmake)

# Set compiler warnings
add_library(project_warnings INTERFACE)
include(cmake/CompilerWarnings.cmake)
set_project_warnings(project_warnings)

# Include supported sanitizers
include(cmake/Sanitizers.cmake)
enable_sanitizers(project_options)

# Configure static analysis
include(cmake/StaticAnalyzers.cmake)

option(ENABLE_TESTING "Enable Test Builds" ON)

# Compile src directory (main code)
add_subdirectory(src)

if(ENABLE_TESTING)
  find_package(Catch2 CONFIG REQUIRED)
  enable_testing()
  add_subdirectory(test)
endif()