cmake_minimum_required(VERSION 2.8.4)

project(fts_remote_console)
set(
source_list
  ../../../../code/lib_fts/fts_fuzzy_match.h
  ../../../../code/lib_fts/fts_sleep.h
  ../../../../code/lib_fts/fts_timer.h
  ../../../../code/remote_console/console.cpp
  ../../../../code/remote_console/console.h
  ../../../../code/remote_console/game_browser.cpp
  ../../../../code/remote_console/game_browser.h
  ../../../../code/remote_console/game_info.cpp
  ../../../../code/remote_console/game_info.h
  ../../../../code/remote_console/high_dpi_handler.cpp
  ../../../../code/remote_console/high_dpi_handler.h
  ../../../../code/remote_console/imgui_ext.cpp
  ../../../../code/remote_console/imgui_ext.h
  ../../../../code/remote_console/imgui_impl_glfw.cpp
  ../../../../code/remote_console/imgui_impl_glfw.h
  ../../../../code/remote_console/remote_console_main.cpp
  ../../../../code/remote_console/tab_bar.cpp
  ../../../../code/remote_console/tab_bar.h
)
include_directories(../../../../code/thirdparty)
include_directories(../../../../code/thirdparty/imgui)
include_directories(../../../../code/thirdparty/glfw/include)
include_directories(../../../../code)
add_definitions(-DASIO_STANDALONE)
add_definitions(-DDEBUG)
add_definitions(-DFTS_WINDOWS)
add_definitions(-D_WIN32_WINNT=0x0601)
add_executable(fts_remote_console ${source_list})
