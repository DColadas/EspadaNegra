find_package(Catch2 REQUIRED)
add_executable(catchTests 
  main.cpp
  testJoinMatchRequest.cpp
  testPlayerAction.cpp
)

target_include_directories(catchTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(catchTests Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(catchTests)
