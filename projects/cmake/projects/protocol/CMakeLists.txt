cmake_minimum_required(VERSION 2.8.4)

project(protocol)
set(
source_list
  ../../../../code/protocol/dummy.cpp
  ../../../../code/protocol/protocol.fbs
  ../../../../code/protocol/protocol_generated.h
)
include_directories(../../../../code/thirdparty)
add_definitions(-DDEBUG)
add_definitions(-DFTS_WINDOWS)
add_definitions(-D_WIN32_WINNT=0x0601)
add_library(protocol STATIC ${source_list})
