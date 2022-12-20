cmake_minimum_required(VERSION 3.10)
project(daw)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")

add_subdirectory(src)
add_subdirectory(test)

add_executable(daw main.cc)

target_link_libraries(daw PUBLIC libdaw)
target_include_directories(daw PUBLIC
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}/src"
)
