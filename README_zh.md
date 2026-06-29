# 通用井字棋（C语言实现）

[![CI](https://github.com/frankwyf/tictactoe-c/actions/workflows/ci.yml/badge.svg)](https://github.com/frankwyf/tictactoe-c/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))

[English](README.md) | **中文** | [日本語](README_ja.md)

用 C11 编写的灵活命令行井字棋游戏。  
支持运行时配置棋盘大小（3–10）和获胜连子数，可以从经典的 3×3 玩到 10×10 的五子棋风格对局。

---

## 特性

- 棋盘大小：**3×3 至 10×10**（运行时可配置）
- 获胜条件：**连续 3 子或更多**（可配置，不超过棋盘大小）
- 双人热座对战（玩家 X 对玩家 O）
- 完整的落子历史记录，对局结束后支持**棋谱回放**
- **高效胜负判定**算法 — 每步 O(获胜长度) 而非 O(n²)
- 27 个单元测试，零外部依赖
- 多平台 CI：Ubuntu · macOS · Windows

---

## 快速开始

### CMake（全平台通用）

```bash
cmake -B build
cmake --build build
```

运行游戏：
- Linux / macOS：`./build/tictactoe`
- Windows：`build\Debug\tictactoe.exe`

### Makefile（Linux / macOS）

```bash
make
./build/tictactoe
```

---

## 构建与运行测试

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

预期输出（27 个测试全部通过）：

```
Results: 27/27 passed
```

---

## 项目结构

```
tictactoe-c/
├── src/
│   ├── game.h        公共 API（类型、常量、函数声明）
│   ├── game.c        核心游戏逻辑（棋盘、落子验证、胜负判定）
│   └── main.c        交互式双人 CLI
├── tests/
│   └── test_game.c   27 个单元测试（内置零依赖测试框架）
├── .github/
│   └── workflows/
│       ├── ci.yml        CI — 构建、测试、静态分析、sanitizer、valgrind
│       └── release.yml   CD — 打包二进制文件并在打标签时发布 GitHub Release
├── CMakeLists.txt    主构建系统
├── Makefile          Linux/macOS 便捷构建
└── ...
```

---

## API 参考

所有公共符号均在 `src/game.h` 中声明。

| 函数 | 说明 | 返回值 |
|------|------|--------|
| `game_init(n, k)` | 初始化 n×n 棋盘，连 k 子获胜 | `OK` / `FAIL` |
| `game_make_move(r, c, sym)` | 在 (row, col) 落子 X 或 O | `OK` / `FAIL` |
| `game_board_is_full()` | 判断是否平局 | `1` / `0` |
| `game_check_horizontal(sym, k)` | 扫描横向连子 | `OK` / `FAIL` / `ERR` |
| `game_check_vertical(sym, k)` | 扫描纵向连子 | `OK` / `FAIL` / `ERR` |
| `game_check_diagonals(sym, k)` | 扫描 ↘ 对角线 | `OK` / `FAIL` / `ERR` |
| `game_check_anti_diagonals(sym, k)` | 扫描 ↙ 反对角线 | `OK` / `FAIL` / `ERR` |
| `game_player_has_won(sym, k)` | 全盘面胜负扫描（四个方向） | `OK` / `FAIL` / `ERR` |
| `game_player_has_won_fast(r, c, sym, k)` | 基于最后落子的高效胜负判定 | `OK` / `FAIL` / `ERR` |
| `game_show_grid()` | 向 stdout 输出当前棋盘 | — |

返回码：`TICTACTOE_OK = 1`，`TICTACTOE_FAIL = 0`，`TICTACTOE_ERR = -1`

---

## 算法说明

`game_player_has_won` 扫描所有行、列和对角线 — 每次调用 **O(n²)**。

`game_player_has_won_fast` 仅检查经过最后落子位置的四个方向：

```
←→  横向
↕   纵向
↘↖  对角线
↙↗  反对角线
```

沿每个方向从新落子向外扩展，直到遇到不匹配的棋子，单次调用时间复杂度为 **O(获胜长度)**。这与人类棋手自然判断胜负的方式完全一致。

---

## 示例对局

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

## 贡献指南

请参阅 [CONTRIBUTING.md](CONTRIBUTING.md)。

---

## 许可证

[MIT](LICENSE) © 2026 frankwyf
