cmake_minimum_required(VERSION 2.8)
project(cbible)
include_directories("/usr/include/sword")
link_directories("/usr/lib")
set(cbible_sources ${CMAKE_CURRENT_SOURCE_DIR}/src/Options.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/src/SwordFuncs.cpp
                   ${CMAKE_CURRENT_SOURCE_DIR}/src/cbible.cpp)
set(gcc_cflags "-O0 -g3 -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${gcc_cflags}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${gcc_cflags}")
add_executable(cbible ${cbible_sources})
target_link_libraries(cbible sword readline)
install(TARGETS cbible RUNTIME DESTINATION bin)
