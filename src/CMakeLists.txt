file(GLOB HEADER_FILES *.hpp)

add_library(meshfiles STATIC polylla.hpp HalfEdge_AoS.hpp)
