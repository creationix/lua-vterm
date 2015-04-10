project(vterm C)

cmake_minimum_required(VERSION 2.6)
message(STATUS "CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}")

find_package(PkgConfig)
pkg_check_modules(LUA luajit)

include_directories(
  libvterm/include
  libvterm/src
  libvterm/src/encoding
  ${LUA_INCLUDE_DIRS}
)

set(CMAKE_C_FLAGS "-g -std=c99 -Wall")

add_library (vterm MODULE vterm.c
  libvterm/src/encoding.c
  libvterm/src/keyboard.c
  libvterm/src/mouse.c
  libvterm/src/parser.c
  libvterm/src/pen.c
  libvterm/src/screen.c
  libvterm/src/state.c
  libvterm/src/unicode.c
  libvterm/src/vterm.c
)
set_target_properties(vterm PROPERTIES OUTPUT_NAME "vterm")
set_target_properties(vterm PROPERTIES PREFIX "")

if(APPLE)
  set(CMAKE_SHARED_MODULE_CREATE_C_FLAGS
    "${CMAKE_SHARED_MODULE_CREATE_C_FLAGS} -flat_namespace -undefined suppress"
  )
endif()

target_link_libraries(vterm)
