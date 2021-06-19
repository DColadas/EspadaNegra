add_library(Manager
  MatchManager.cpp
  MatchManager.hpp
)

target_link_libraries(Manager 
  PRIVATE 
  Events 
  IO
  Logging
  Utils
)

target_link_libraries(Manager 
  PUBLIC 
  Game
)
