cmake_minimum_required(VERSION 2.8.4)

project(glfw)
set(
source_list
  ../../../../code/thirdparty/glfw/include/GLFW/glfw3.h
  ../../../../code/thirdparty/glfw/include/GLFW/glfw3native.h
  ../../../../code/thirdparty/glfw/src/context.c
  ../../../../code/thirdparty/glfw/src/egl_context.c
  ../../../../code/thirdparty/glfw/src/egl_context.h
  ../../../../code/thirdparty/glfw/src/glfw_config.h
  ../../../../code/thirdparty/glfw/src/init.c
  ../../../../code/thirdparty/glfw/src/input.c
  ../../../../code/thirdparty/glfw/src/internal.h
  ../../../../code/thirdparty/glfw/src/monitor.c
  ../../../../code/thirdparty/glfw/src/vulkan.c
  ../../../../code/thirdparty/glfw/src/wgl_context.c
  ../../../../code/thirdparty/glfw/src/wgl_context.h
  ../../../../code/thirdparty/glfw/src/win32_init.c
  ../../../../code/thirdparty/glfw/src/win32_joystick.c
  ../../../../code/thirdparty/glfw/src/win32_joystick.h
  ../../../../code/thirdparty/glfw/src/win32_monitor.c
  ../../../../code/thirdparty/glfw/src/win32_platform.h
  ../../../../code/thirdparty/glfw/src/win32_time.c
  ../../../../code/thirdparty/glfw/src/win32_tls.c
  ../../../../code/thirdparty/glfw/src/win32_window.c
  ../../../../code/thirdparty/glfw/src/window.c
)
include_directories(../../../../code/thirdparty)
add_definitions(-D_GLFW_WIN32)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
add_definitions(-DDEBUG)
add_definitions(-DFTS_WINDOWS)
add_definitions(-D_WIN32_WINNT=0x0601)
add_library(glfw STATIC ${source_list})
