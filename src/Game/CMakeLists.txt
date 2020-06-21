add_library(Game
  Card.hpp
  Card.cpp
  CardModel.hpp
  CardModel.cpp
  Deck.cpp
  Deck.hpp
  GameModifier.hpp
  GameParameter.hpp
  Match.cpp
  Match.hpp
  MatchConfig.cpp
  MatchConfig.hpp
  MatchHandler.cpp
  MatchHandler.hpp
  Phase.hpp
  PhaseHandler.cpp
  PhaseHandler.hpp
  Player.cpp
  Player.hpp
)

target_link_libraries(Game PRIVATE Utils Logging)
