file(GLOB HEADER_FILES *.hpp)

add_library(meshfiles STATIC ${HEADER_FILES})
