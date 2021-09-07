cmake_minimum_required(VERSION 3.8)
project (SnazeGame VERSION 1.0.0 LANGUAGES CXX )

set_property(GLOBAL PROPERTY CXX_STANDARD 17)

include_directories( include )
add_executable(snaze src/main.cpp
                     src/snaze_manager.cpp
                     src/snake.cpp )

set(CMAKE_CXX_FLAGS "-g")
