add_library(Parsing
    JSONParser.cpp
    JSONParser.hpp
)

find_package(Boost REQUIRED)
target_include_directories(Parsing SYSTEM PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(Parsing 
  PRIVATE 
  ${Boost_LIBRARIES} 
  Logging 
  Utils 
  Game
)
