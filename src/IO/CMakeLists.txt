add_library(IO
  Asio.hpp
  Beast.hpp
  IOHandler.hpp
  Listener.cpp
  Listener.hpp
  MatchManager.cpp
  MatchManager.hpp
  Server.hpp
  Server.cpp
  WebsocketHandler.cpp
  WebsocketHandler.hpp
  WebsocketSession.cpp
  WebsocketSession.hpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

find_package(Threads REQUIRED)

target_link_libraries(IO 
PRIVATE 
  ${Boost_LIBRARIES} 
  Threads::Threads 
  Events 
  Game 
  Logging
  Parsing
)
