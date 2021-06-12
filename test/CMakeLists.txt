add_executable(catchTests 
  main.cpp
  testComplex.cpp
  testJSONParser.cpp
)

find_package(Boost REQUIRED)
target_include_directories(catchTests SYSTEM PRIVATE ${Boost_INCLUDE_DIR})

target_include_directories(catchTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(catchTests 
  Catch2::Catch2 
  ${Boost_LIBRARIES}
  Events
  Game
  Parsing
)

include(CTest)
