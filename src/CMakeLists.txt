include_directories(.)
add_subdirectory(Events)
add_subdirectory(Game)
add_subdirectory(IO)
add_subdirectory(Logging)
add_subdirectory(Parsing)
add_subdirectory(Utils)
add_executable(espadaNegra main.cpp)

target_link_libraries(espadaNegra 
PRIVATE 
  Events
  Game
  IO 
  Logging
  Parsing
  Utils
)
