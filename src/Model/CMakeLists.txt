add_library(Model
  Card.hpp
  Card.cpp
  CardModel.hpp
  CardModel.cpp
  Deck.cpp
  Deck.hpp
  GameModifier.hpp
  GameParameter.hpp
  MatchConfig.cpp
  MatchConfig.hpp
  Player.cpp
  Player.hpp
)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(Model 
  PRIVATE 
  Utils 
  Logging 
)

target_link_libraries(Model 
  PUBLIC 
  nlohmann_json::nlohmann_json
)