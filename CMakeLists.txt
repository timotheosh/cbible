cmake_minimum_required(VERSION 3.20)
project(cbible VERSION 0.20 LANGUAGES CXX)

include(CTest)
include(GNUInstallDirs)
include(FindPkgConfig)

option(CBIBLE_ENABLE_SANITIZERS "Enable address and undefined-behavior sanitizers" OFF)
option(CBIBLE_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)

pkg_check_modules(SWORD REQUIRED IMPORTED_TARGET sword)
pkg_check_modules(READLINE REQUIRED IMPORTED_TARGET readline)

add_library(cbible_core src/Options.cpp src/SwordFuncs.cpp src/Output.cpp)
target_include_directories(cbible_core PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_compile_features(cbible_core PUBLIC cxx_std_20)
target_link_libraries(cbible_core PUBLIC PkgConfig::SWORD)

add_executable(cbible src/cbible.cpp)
target_compile_features(cbible PRIVATE cxx_std_20)
target_link_libraries(cbible PRIVATE cbible_core PkgConfig::READLINE)
target_compile_definitions(cbible PRIVATE CBIBLE_VERSION="${PROJECT_VERSION}")

set(cbible_warning_flags -Wall -Wextra -Wpedantic -Wconversion -Wshadow)
if(CBIBLE_WARNINGS_AS_ERRORS)
  list(APPEND cbible_warning_flags -Werror)
endif()
target_compile_options(cbible_core PRIVATE ${cbible_warning_flags})
target_compile_options(cbible PRIVATE ${cbible_warning_flags})

if(CBIBLE_ENABLE_SANITIZERS)
  if(NOT CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    message(FATAL_ERROR "Sanitizers require Clang or GCC")
  endif()
  target_compile_options(cbible_core PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(cbible_core PUBLIC -fsanitize=address,undefined)
  target_compile_options(cbible PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
  target_link_options(cbible PRIVATE -fsanitize=address,undefined)
endif()

install(TARGETS cbible RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
install(FILES elisp/cbible.el DESTINATION "${CMAKE_INSTALL_DATADIR}/emacs/site-lisp")

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
