cmake_minimum_required(VERSION 2.8.4)

project(flatbuffers)
set(
source_list
  ../../../../code/thirdparty/flatbuffers/code_generators.h
  ../../../../code/thirdparty/flatbuffers/flatbuffers.h
  ../../../../code/thirdparty/flatbuffers/flatc.cpp
  ../../../../code/thirdparty/flatbuffers/hash.h
  ../../../../code/thirdparty/flatbuffers/idl.h
  ../../../../code/thirdparty/flatbuffers/idl_gen_cpp.cpp
  ../../../../code/thirdparty/flatbuffers/idl_gen_fbs.cpp
  ../../../../code/thirdparty/flatbuffers/idl_gen_general.cpp
  ../../../../code/thirdparty/flatbuffers/idl_gen_text.cpp
  ../../../../code/thirdparty/flatbuffers/idl_parser.cpp
  ../../../../code/thirdparty/flatbuffers/reflection.h
  ../../../../code/thirdparty/flatbuffers/reflection_generated.h
  ../../../../code/thirdparty/flatbuffers/util.cpp
  ../../../../code/thirdparty/flatbuffers/util.h
)
include_directories(../../../../code/thirdparty)
add_definitions(-DFLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE)
add_definitions(-DDEBUG)
add_definitions(-DFTS_WINDOWS)
add_definitions(-D_WIN32_WINNT=0x0601)
add_executable(flatbuffers ${source_list})
