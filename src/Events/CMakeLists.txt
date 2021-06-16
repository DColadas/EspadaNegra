add_library(Events
    AttackResult.hpp
    Common.hpp
    Complex.hpp
    Draw.hpp
    Earn.hpp
    Error.hpp
    GetCard.hpp
    InputEvent.cpp
    InputEvent.hpp
    IsAuctioneer.hpp
    JoinMatchResult.hpp
    Leave.hpp
    MatchInfo.hpp
    OfferResult.hpp
    OutputEvent.hpp
    PassResult.hpp
    Pay.hpp
    PlayerResult.hpp
    SetGold.hpp
    Winner.hpp
)

find_package(nlohmann_json CONFIG REQUIRED)

target_link_libraries(Events 
PRIVATE 
  Utils 
  nlohmann_json::nlohmann_json
)
