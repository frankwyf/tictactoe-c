# Changelog

All notable changes to this project will be documented in this file.

Format follows [Keep a Changelog](https://keepachangelog.com/) and
[Semantic Versioning](https://semver.org/).

---

## [1.0.0] — 2026-06-30

### Added

**Core game library (`src/game.c` / `src/game.h`)**
- `game_init(n, k)` — initialize an n×n board with win condition k
- `game_make_move(r, c, sym)` — place a symbol with full input validation
- `game_board_is_full()` — draw detection
- `game_check_horizontal/vertical/diagonals/anti_diagonals()` — directional win scanners
- `game_player_has_won(sym, k)` — full-board O(n²) win scan
- `game_player_has_won_fast(r, c, sym, k)` — incremental O(win_length) win check anchored at the last move
- `game_show_grid()` — formatted board display

**Interactive CLI (`src/main.c`)**
- Two-player hot-seat mode with input validation and re-prompting
- Move history recording
- Post-game replay (step through every move)

**Tests (`tests/test_game.c`)**
- 27 unit tests covering init, move validation, all four win-check directions,
  the full-scan and fast-check functions, edge cases, and cross-function consistency

**Build system**
- CMake 3.14+ with `tictactoe_game` static library and `tictactoe` / `test_game` targets
- Makefile for Linux/macOS
- CMake install rules for binary and header

**CI/CD (`.github/workflows/`)**
- `ci.yml` — matrix build on Ubuntu / macOS / Windows; cppcheck static analysis;
  AddressSanitizer + UBSanitizer; Valgrind leak check
- `release.yml` — automated cross-platform release binaries on git tag push

**Documentation**
- `README.md` (English)
- `README_zh.md` (Chinese / 中文)
- `README_ja.md` (Japanese / 日本語)
- `CONTRIBUTING.md`
- `CHANGELOG.md`
- MIT `LICENSE`
- `.gitignore`
