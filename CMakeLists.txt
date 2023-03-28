cmake_minimum_required(VERSION 3.4)

project(Polylla)

add_subdirectory(src)
include_directories(src)

set(Polylla -O3)

add_executable(Polylla main.cpp)

target_link_libraries(Polylla PUBLIC meshfiles)
set_target_properties(meshfiles PROPERTIES LINKER_LANGUAGE CXX)
