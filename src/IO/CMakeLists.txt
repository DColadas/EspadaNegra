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
find_package(Threads REQUIRED)
target_include_directories(IO SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(IO 
  PRIVATE 
  ${Boost_LIBRARIES} 
  Threads::Threads 
  Game 
  Logging
)

target_link_libraries(IO 
  PUBLIC 
  Events 
)
