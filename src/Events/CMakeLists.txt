add_library(Events
    Attack.hpp
    GameEvent.hpp
    JoinMatchRequest.hpp
    Offer.hpp
    Pass.hpp
    PlayerAction.hpp
    PlayerEvent.hpp
    Time.hpp
)

set_target_properties(Events PROPERTIES LINKER_LANGUAGE CXX)
