add_executable(catchTests 
  main.cpp
  testEvents.cpp
)

target_include_directories(catchTests PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(catchTests 
  Catch2::Catch2 
  Events
  Model
)

include(CTest)
