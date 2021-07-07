add_library(Manager
  MatchManager.hpp
  NoopManager.hpp
  ServerManager.cpp
  ServerManager.hpp
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
