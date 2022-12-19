cmake_minimum_required(VERSION 3.14)
project(daw_test)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  daw_test
  test.cc
)
target_link_libraries(
  daw_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(daw_test)

