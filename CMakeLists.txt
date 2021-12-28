cmake_minimum_required(VERSION 3.18)

project(Polylla)

add_subdirectory(src)
include_directories(src)

add_executable(Polylla main.cpp)

target_link_libraries(Polylla PUBLIC meshfiles)
set_target_properties(meshfiles PROPERTIES LINKER_LANGUAGE CXX)
