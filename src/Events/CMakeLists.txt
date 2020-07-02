add_library(Events
    AttackRequest.hpp
    AttackResult.hpp
    Draw.hpp
    Earn.hpp
    Error.hpp
    GetCard.hpp
    IsAuctioneer.hpp
    JoinMatchRequest.hpp
    JoinMatchResult.hpp
    JSONParser.cpp
    JSONParser.hpp
    Leave.hpp
    MatchInfo.hpp
    OfferRequest.hpp
    OfferResult.hpp
    OutputEvent.hpp
    PassRequest.hpp
    PassResult.hpp
    Pay.hpp
    InputEvent.hpp
    PlayerResult.hpp
    SetGold.hpp
    Winner.hpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(Events PRIVATE ${Boost_LIBRARIES} Logging Utils)
