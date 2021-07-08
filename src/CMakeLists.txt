include_directories(.)
add_subdirectory(Events)
add_subdirectory(Game)
add_subdirectory(IO)
add_subdirectory(Logging)
add_subdirectory(Manager)
add_subdirectory(Model)
add_subdirectory(Utils)

if(${EMSCRIPTEN})
  find_package(nlohmann_json CONFIG REQUIRED)
  add_executable(espadaNegraClient EmscriptenBindings.cpp)
  set_target_properties(espadaNegraClient PROPERTIES LINK_FLAGS "--bind --std=c++2a -s MODULARIZE=1 -s EXPORT_NAME='createModule' -s WASM=0 -s ENVIRONMENT=web --memory-init-file 0")
  target_link_libraries(espadaNegraClient 
    PRIVATE 
    Events
    Game
    Manager
    Model
    Utils
    nlohmann_json::nlohmann_json
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
