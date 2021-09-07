cmake_minimum_required(VERSION 3.5)
project (HashTable VERSION 1.0.0 LANGUAGES CXX )

include_directories( include )
add_executable(snaze src/main.cpp
                     src/snaze_manager.cpp
                     src/snake.cpp )

set(CMAKE_CXX_FLAGS "-g")
