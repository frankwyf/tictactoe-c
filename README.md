# Generalized Tic-Tac-Toe (C)

[![CI](https://github.com/frankwyf/tictactoe-c/actions/workflows/ci.yml/badge.svg)](https://github.com/frankwyf/tictactoe-c/actions/workflows/ci.yml)
[![codecov](https://codecov.io/gh/frankwyf/tictactoe-c/branch/main/graph/badge.svg)](https://codecov.io/gh/frankwyf/tictactoe-c)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![GitHub release](https://img.shields.io/github/v/release/frankwyf/tictactoe-c)](https://github.com/frankwyf/tictactoe-c/releases)

**English** | [中文](README_zh.md) | [日本語](README_ja.md)

A flexible, command-line Tic-Tac-Toe game written in C11.  
Supports configurable grid size (3–10) and win condition length — from the classic 3×3 to a 10×10 Gomoku-style game.

---

## Features

- Grid size: **3×3 up to 10×10** (configurable at runtime)
- Win length: **3 or more in a row** (configurable, up to grid size)
- Two-player hot-seat mode (Player X vs Player O)
- Full move-history recording with **game-replay** after match ends
- **Efficient win-detection** algorithm — O(win_length) per move instead of O(n²)
- 27 unit tests, zero external dependencies
- Multi-platform CI: Ubuntu · macOS · Windows

---

## Quick Start

### CMakePresets (recommended)

```bash
cmake --preset debug          # configure
cmake --build --preset debug  # build
ctest --preset debug          # test
```

Available presets: `debug` · `release` · `coverage` (Linux/macOS only)

### CMake (manual)

```bash
cmake -B build
cmake --build build
```

Run the game:
- Linux / macOS: `./build/tictactoe`
- Windows:       `build\Debug\tictactoe.exe`

### Makefile (Linux / macOS)

```bash
make
./build/tictactoe
```

---

## Building & Running Tests

```bash
# CMakePresets (recommended)
cmake --preset debug && cmake --build --preset debug
ctest --preset debug

# Or manual CMake
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Expected output (all 83 assertions pass across 27 test groups):

```
Results: 83/83 passed
```

### Code Coverage (Linux / macOS)

```bash
cmake --preset coverage && cmake --build --preset coverage
ctest --preset coverage
# Then collect with lcov / genhtml or upload to Codecov via CI
```

---

## Project Structure

```
tictactoe-c/
├── src/
│   ├── game.h        Public API (types, constants, function declarations)
│   ├── game.c        Core game logic (board, move validation, win detection)
│   └── main.c        Interactive two-player CLI
├── examples/
│   └── sample_game.c Scripted demo — shows API usage without stdin
├── tests/
│   └── test_game.c   83 assertions across 27 test groups (built-in runner)
├── .github/
│   ├── ISSUE_TEMPLATE/     Bug report & feature request forms
│   ├── PULL_REQUEST_TEMPLATE.md
│   └── workflows/
│       ├── ci.yml     CI: build·test·cppcheck·ASan·valgrind·coverage·format
│       └── release.yml  CD: cross-platform binaries on tag push
├── CMakeLists.txt    Primary build system
├── CMakePresets.json debug / release / coverage presets
├── Makefile          Convenience build for Linux/macOS
├── .clang-format     Code style (LLVM-based, 4-space, Allman functions)
└── ...
```

---

## API Reference

All public symbols live in `src/game.h`.

| Function | Description | Returns |
|----------|-------------|---------|
| `game_init(n, k)` | Initialize n×n board, win on k consecutive | `OK` / `FAIL` |
| `game_make_move(r, c, sym)` | Place X or O at (row, col) | `OK` / `FAIL` |
| `game_board_is_full()` | Check draw condition | `1` / `0` |
| `game_check_horizontal(sym, k)` | Scan board for horizontal run | `OK` / `FAIL` / `ERR` |
| `game_check_vertical(sym, k)` | Scan board for vertical run | `OK` / `FAIL` / `ERR` |
| `game_check_diagonals(sym, k)` | Scan ↘ diagonals | `OK` / `FAIL` / `ERR` |
| `game_check_anti_diagonals(sym, k)` | Scan ↙ anti-diagonals | `OK` / `FAIL` / `ERR` |
| `game_player_has_won(sym, k)` | Full-board win scan (all 4 directions) | `OK` / `FAIL` / `ERR` |
| `game_player_has_won_fast(r, c, sym, k)` | Win check anchored at last move | `OK` / `FAIL` / `ERR` |
| `game_show_grid()` | Print current board to stdout | — |

Return codes: `TICTACTOE_OK = 1`, `TICTACTOE_FAIL = 0`, `TICTACTOE_ERR = -1`

---

## Algorithm

`game_player_has_won` scans every row, column, and diagonal — **O(n²)** per call.

`game_player_has_won_fast` only examines the four lines that pass through the
most-recently-placed cell:

```
←→  horizontal
↕   vertical
↘↖  diagonal
↙↗  anti-diagonal
```

Each direction is walked outward from the new cell until a mismatch is found,
giving **O(win_length)** per call. This is the same strategy a human player
naturally uses when checking for a win.

---

## Sample Session

```
=== Tic-Tac-Toe ===

Enter grid size   (3-10): 3
Enter winning length (3-3): 3

	0 1 2
0	. . .
1	. . .
2	. . .

Player X, Choose Location (row col): 1 1
...
**************************
Player X has won the game
**************************
```

---

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md).

---

## License

[MIT](LICENSE) © 2026 frankwyf
