cmake_minimum_required(VERSION 2.8.4)

project(example_game)
set(
source_list
  ../../../../code/example_game/example_game_main.cpp
)
include_directories(../../../../code/thirdparty)
include_directories(../../../../code/thirdparty/asio)
include_directories(../../../../code)
add_definitions(-DASIO_STANDALONE)
add_definitions(-DDEBUG)
add_definitions(-DFTS_WINDOWS)
add_definitions(-D_WIN32_WINNT=0x0601)
add_executable(example_game ${source_list})
