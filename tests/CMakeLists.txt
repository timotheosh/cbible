include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.8.1
  GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Catch2)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/swordtest/sword"
     DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/fixtures")
file(CHMOD_RECURSE
  "${CMAKE_CURRENT_BINARY_DIR}/fixtures/sword/modules/texts/ztext/kjv"
  "${CMAKE_CURRENT_BINARY_DIR}/fixtures/sword/mods.d/kjv.conf"
  PERMISSIONS OWNER_READ OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

add_executable(cbibleTests cbibleTests.cpp)
target_compile_features(cbibleTests PRIVATE cxx_std_20)
target_link_libraries(cbibleTests PRIVATE cbible_core Catch2::Catch2WithMain)
target_compile_options(cbibleTests PRIVATE -Wall -Wextra -Wpedantic -Wconversion -Wshadow)

include(Catch)
catch_discover_tests(cbibleTests
  DISCOVERY_MODE PRE_TEST
  PROPERTIES ENVIRONMENT "SWORD_PATH=${CMAKE_CURRENT_BINARY_DIR}/fixtures/sword")

add_test(NAME cli-compatibility
  COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/cli_tests.sh" "$<TARGET_FILE:cbible>"
          "${CMAKE_CURRENT_BINARY_DIR}/fixtures/sword")

find_program(EMACS_EXECUTABLE emacs)
if(EMACS_EXECUTABLE)
  add_test(NAME emacs-mode
    COMMAND "${EMACS_EXECUTABLE}" --batch -Q
            -L "${CMAKE_CURRENT_SOURCE_DIR}/../elisp"
            -l "${CMAKE_CURRENT_SOURCE_DIR}/cbible-ert-tests.el"
            -f ert-run-tests-batch-and-exit)
else()
  message(STATUS "Emacs not found; ERT tests will not run")
endif()
