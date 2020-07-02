add_library(Events
    AttackRequest.hpp
    Earn.hpp
    GameEvent.hpp
    GetCard.hpp
    JoinMatchRequest.hpp
    JSONParser.cpp
    JSONParser.hpp
    OfferRequest.hpp
    PassRequest.hpp
    Pay.hpp
    InputEvent.hpp
    PlayerEvent.hpp
    PlayerResult.hpp
    Winner.hpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(Events PRIVATE ${Boost_LIBRARIES} Logging Utils)
