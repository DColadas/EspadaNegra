add_library(Events
    Attack.hpp
    GameEvent.hpp
    JoinMatchRequest.hpp
    JSONParser.hpp
    Offer.hpp
    Pass.hpp
    PlayerAction.hpp
    PlayerEvent.hpp
    Time.hpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(Events PRIVATE ${Boost_LIBRARIES})

set_target_properties(Events PROPERTIES LINKER_LANGUAGE CXX)