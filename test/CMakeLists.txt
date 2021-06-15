add_executable(catchTests 
  main.cpp
  #testComplex.cpp
  #testJSONParser.cpp
  testInputEvent.cpp
)

find_package(Boost REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
target_include_directories(catchTests SYSTEM PRIVATE ${Boost_INCLUDE_DIR})

target_include_directories(catchTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(catchTests 
  Catch2::Catch2 
  ${Boost_LIBRARIES}
  nlohmann_json::nlohmann_json
  Events
  # Game
  # Parsing
)

include(CTest)
