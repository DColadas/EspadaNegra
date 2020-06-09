add_library(IO
  Asio.hpp
  CMakeLists.txt
  Listener.cpp
  MatchManager.cpp
  Server.cpp
  WebsocketHandler.cpp
  WebsocketSession.cpp
  Beast.hpp
  IOHandler.hpp
  Listener.hpp
  MatchManager.hpp
  Server.hpp
  WebsocketHandler.hpp
  WebsocketSession.hpp
)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(IO PRIVATE ${Boost_LIBRARIES})

#set_target_properties(IO PROPERTIES LINKER_LANGUAGE CXX)
