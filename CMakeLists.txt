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

find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 REQUIRED x11)
pkg_check_modules(Cairo REQUIRED cairo)
pkg_check_modules(PA_Simple REQUIRED libpulse-simple)

target_link_libraries(daw PRIVATE ${X11_LIBRARIES})

target_link_libraries(daw PRIVATE ${Cairo_LIBRARIES})
target_include_directories(daw PRIVATE ${Cairo_INCLUDE_DIRS})

target_link_libraries(daw PRIVATE ${PA_Simple_LIBRARIES})

