include_directories(.)
add_subdirectory(Events)
add_subdirectory(Game)
add_subdirectory(IO)
add_subdirectory(Logging)
add_subdirectory(Manager)
add_subdirectory(Model)
add_subdirectory(Utils)

if(${EMSCRIPTEN})
  add_executable(espadaNegraClient EmscriptenBindings.cpp)
  set_target_properties(espadaNegraClient PROPERTIES LINK_FLAGS "--bind --std=c++2a -s MODULARIZE=1 -s EXPORT_NAME='createModule' -s WASM=0 -s ENVIRONMENT=web --memory-init-file 0")
  target_link_libraries(espadaNegraClient 
    PRIVATE 
    Events
    Utils
  )
else()
  add_executable(espadaNegraServer main.cpp)
  target_link_libraries(espadaNegraServer 
    PRIVATE 
    Events
    Game
    IO 
    Logging
    Manager
    Model
    Utils
  )
endif()
