cmake_minimum_required(VERSION 2.8)
project(cbibleTests)
include_directories("/usr/include/sword")
link_directories("/usr/lib")
set(gcc_cflags "-g3 -std=c++11")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${gcc_cflags}")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${gcc_cflags}")
set(linklibs "sword" "readline" "boost_program_options")
add_executable(cbibleTests "cbibleTests.cpp" "../src/SwordFuncs.cpp")
target_include_directories(cbibleTests PUBLIC
  "${CMAKE_CURRENT_SOURCE_DIR}/../src"
  "${CMAKE_CURRENT_SOURCE_DIR}/tests/include")
target_link_libraries(cbibleTests sword readline boost_program_options)
install(TARGETS cbibleTests RUNTIME DESTINATION bin)
