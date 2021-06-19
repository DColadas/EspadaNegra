add_library(IO
  Asio.hpp
  Beast.hpp
  IOHandler.cpp
  IOHandler.hpp
  Listener.cpp
  Listener.hpp
  Server.hpp
  Server.cpp
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
  Logging
)

target_link_libraries(IO 
  PUBLIC 
  Manager
)