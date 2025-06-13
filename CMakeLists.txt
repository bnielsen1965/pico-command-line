
add_library(pico-command-line INTERFACE)

target_sources(pico-command-line INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/pico-command-line.c
)

target_include_directories(pico-command-line INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/src/include
)

target_link_libraries(pico-command-line INTERFACE
  pico_stdlib
)
