cmake_minimum_required(VERSION 3.16.3)
project(steam-input C)

add_subdirectory(deps/piot/clog/src/lib)
add_subdirectory(deps/piot/tiny-libc/src/lib)
add_subdirectory(deps/piot/steam-api-c/src/lib)
add_subdirectory(deps/piot/atheneum/src/lib)



add_subdirectory(lib)



