include_directories(.)
add_subdirectory(Events)
add_subdirectory(Game)
add_subdirectory(IO)
add_subdirectory(Logging)
add_subdirectory(Manager)
add_subdirectory(Model)
add_subdirectory(Utils)

add_executable(espadaNegra EmscriptenBindings.cpp)
set_target_properties(espadaNegra PROPERTIES LINK_FLAGS "--bind --std=c++2a  -s MODULARIZE=1 -s EXPORT_NAME='createModule' -s WASM=0 -s ENVIRONMENT=web --no-entry -s ERROR_ON_UNDEFINED_SYMBOLS=0")

target_link_libraries(espadaNegra 
  PRIVATE 
  Events
  Utils
)
