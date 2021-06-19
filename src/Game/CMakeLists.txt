add_library(Game
  Match.cpp
  Match.hpp
  MatchHandler.cpp
  MatchHandler.hpp
  Phase.hpp
  PhaseHandler.cpp
  PhaseHandler.hpp
  Table.cpp
  Table.hpp
)

target_link_libraries(Game 
  PRIVATE 
  Utils 
  Logging 
  Manager
)

target_link_libraries(Game 
  PUBLIC 
  Events
  Model
)