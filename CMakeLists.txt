project(HttpServer)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(./src SRC_LIST)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -Wextra -Woverloaded-virtual -Wunreachable-code -pedantic -lrt -lpthread")
LINK_DIRECTORIES(/usr/local/lib/)


add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} ev)
