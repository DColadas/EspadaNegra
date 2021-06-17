add_library(Logging
  Logger.cpp
  Logger.hpp
)

target_link_libraries(Logging
  PRIVATE
  Utils
)
