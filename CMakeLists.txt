cmake_minimum_required(VERSION 3.14)

project(tictactoe
    VERSION 1.0.0
    DESCRIPTION "Generalized Tic-Tac-Toe — configurable grid (3-10) and win length"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# ── Compiler warnings ────────────────────────────────────────────────────────
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ── Game library ─────────────────────────────────────────────────────────────
add_library(tictactoe_game STATIC
    src/game.c
    src/ai.c
)
target_include_directories(tictactoe_game
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
)

# ── Main executable ───────────────────────────────────────────────────────────
add_executable(tictactoe src/main.c)
target_link_libraries(tictactoe PRIVATE tictactoe_game)

# ── Tests ─────────────────────────────────────────────────────────────────────
enable_testing()

add_executable(test_game tests/test_game.c)
target_link_libraries(test_game PRIVATE tictactoe_game)

add_test(NAME GameTests COMMAND test_game)
set_tests_properties(GameTests PROPERTIES TIMEOUT 30)

add_executable(test_ai tests/test_ai.c)
target_link_libraries(test_ai PRIVATE tictactoe_game)

add_test(NAME AITests COMMAND test_ai)
set_tests_properties(AITests PROPERTIES TIMEOUT 60)

# ── Examples ─────────────────────────────────────────────────────────────────
add_executable(sample_game examples/sample_game.c)
target_link_libraries(sample_game PRIVATE tictactoe_game)

# ── Code coverage (GCC / Clang, Linux / macOS) ────────────────────────────────
option(TICTACTOE_COVERAGE "Enable gcov/lcov code-coverage instrumentation" OFF)
if(TICTACTOE_COVERAGE)
    if(NOT CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(WARNING "TICTACTOE_COVERAGE requires GCC or Clang — disabling.")
        set(TICTACTOE_COVERAGE OFF)
    else()
        message(STATUS "Coverage instrumentation enabled")
        add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
        add_link_options(--coverage)
    endif()
endif()

# ── Install rules ─────────────────────────────────────────────────────────────
include(GNUInstallDirs)

install(TARGETS tictactoe
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(FILES src/game.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tictactoe
)
install(FILES examples/sample_game.c
    DESTINATION ${CMAKE_INSTALL_DOCDIR}/examples
)
