add_library(libdaw xwindow.cc header.cc 
  track.cc grid.cc pcm.cc pahandler.cc)

find_package(PkgConfig REQUIRED)
pkg_check_modules(X11 REQUIRED x11)
pkg_check_modules(Cairo REQUIRED cairo)
pkg_check_modules(PA_Simple REQUIRED libpulse-simple)

target_link_libraries(libdaw PRIVATE ${X11_LIBRARIES})

target_link_libraries(libdaw PRIVATE ${Cairo_LIBRARIES})
target_include_directories(libdaw PRIVATE ${Cairo_INCLUDE_DIRS})

target_link_libraries(libdaw PRIVATE ${PA_Simple_LIBRARIES})

