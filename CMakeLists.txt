PROJECT(vterm)

cmake_minimum_required(VERSION 2.6)
MESSAGE(STATUS "CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}")

INCLUDE_DIRECTORIES(libvterm/include libvterm/src libvterm/src/encoding)

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

target_link_libraries(vterm)
