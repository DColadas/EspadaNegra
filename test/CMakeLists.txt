add_executable(catchTests 
  main.cpp
  testJoinMatchRequest.cpp
  testJSONParser.cpp
  testInputEvent.cpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

target_include_directories(catchTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(catchTests 
  Catch2::Catch2 
  ${Boost_LIBRARIES}
  Events
)

include(CTest)
catch_discover_tests(catchTests)
