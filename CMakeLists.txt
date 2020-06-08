cmake_minimum_required(VERSION 3.10)

project(EspadaNegra CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_TESTING "Enable Test Builds" ON)

add_subdirectory(src)

if(ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()