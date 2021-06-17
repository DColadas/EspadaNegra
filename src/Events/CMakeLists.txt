add_library(Events
    Common.hpp
    InputEvent.cpp
    InputEvent.hpp
    OutputEvent.cpp
    OutputEvent.hpp
)

find_package(nlohmann_json CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

target_link_libraries(Events 
  PRIVATE 
  Utils 
  fmt::fmt
)

target_link_libraries(Events
  PUBLIC
  nlohmann_json::nlohmann_json
)
