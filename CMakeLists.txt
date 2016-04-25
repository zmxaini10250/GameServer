cmake_minimum_required(VERSION 3.5)
project(Server)

SET(CMAKE_C_COMPILER clang)
SET(CMAKE_CXX_COMPILER clang++)

add_definitions(-g -std=c++11)

aux_source_directory(./src/TCPServer/ SRC_LIST)
#aux_source_directory(./src/protobuf/ SRC_LIST)

aux_source_directory(./src SRC_LIST)


#aux_source_directory(/usr/lib/x86_64-linux-gnu LIB_LIST)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_executable(Server ${SRC_LIST})

#target_link_libraries(Server ${LIB_LIST})

