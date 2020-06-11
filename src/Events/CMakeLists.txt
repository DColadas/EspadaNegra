add_library(Events
    Attack.hpp
    Earn.hpp
    GameEvent.hpp
    GetCard.hpp
    JoinMatchRequest.hpp
    JSONParser.cpp
    JSONParser.hpp
    Offer.hpp
    Pass.hpp
    Pay.hpp
    PlayerAction.hpp
    PlayerEvent.hpp
    PlayerResult.hpp
    Time.hpp
    Winner.hpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(Events PRIVATE ${Boost_LIBRARIES})
