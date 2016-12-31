cmake_minimum_required(VERSION 2.8.4)

project(imgui)
set(
source_list
  ../../../../code/thirdparty/imgui/imconfig.h
  ../../../../code/thirdparty/imgui/imgui.cpp
  ../../../../code/thirdparty/imgui/imgui.h
  ../../../../code/thirdparty/imgui/imgui_demo.cpp
  ../../../../code/thirdparty/imgui/imgui_draw.cpp
  ../../../../code/thirdparty/imgui/imgui_internal.h
  ../../../../code/thirdparty/imgui/stb_rect_pack.h
  ../../../../code/thirdparty/imgui/stb_textedit.h
  ../../../../code/thirdparty/imgui/stb_truetype.h
)
include_directories(../../../../code/thirdparty)
add_definitions(-DDEBUG)
add_definitions(-DFTS_WINDOWS)
add_definitions(-D_WIN32_WINNT=0x0601)
add_library(imgui STATIC ${source_list})
