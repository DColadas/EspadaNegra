cmake_minimum_required(VERSION 3.11)

project(EspadaNegra CXX)
include(cmake/StandardProjectSettings.cmake)
add_library(project_options INTERFACE)

# Set used C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# These option gives an error with Boost
#target_compile_features(project_options INTERFACE cxx_std_17)

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
  # Find Catch2 in the system, or download otherwise
  include(cmake/PrepareCatch2.cmake)
  enable_testing()
  add_subdirectory(test)
endif()