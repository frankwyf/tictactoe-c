# 汎用三目並べ（C言語実装）

[![CI](https://github.com/frankwyf/tictactoe-c/actions/workflows/ci.yml/badge.svg)](https://github.com/frankwyf/tictactoe-c/actions/workflows/ci.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![C Standard](https://img.shields.io/badge/C-C11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))

[English](README.md) | [中文](README_zh.md) | **日本語**

C11 で書かれた柔軟なコマンドライン三目並べゲームです。  
グリッドサイズ（3〜10）と勝利条件（連続する駒の数）を実行時に設定でき、クラシックな 3×3 から 10×10 の五目並べスタイルまで幅広く対応します。

---

## 特徴

- グリッドサイズ：**3×3 〜 10×10**（実行時に設定可能）
- 勝利条件：**3個以上の連続**（グリッドサイズまで設定可能）
- 2人対戦（ホットシート）モード（プレイヤー X 対プレイヤー O）
- 全着手履歴を記録し、対局終了後に**棋譜を再生**可能
- **効率的な勝利判定**アルゴリズム — 1手あたり O(n²) ではなく O(勝利条件長)
- 27個のユニットテスト、外部依存なし
- マルチプラットフォーム CI：Ubuntu · macOS · Windows

---

## クイックスタート

### CMake（全プラットフォーム共通）

```bash
cmake -B build
cmake --build build
```

ゲームを起動：
- Linux / macOS：`./build/tictactoe`
- Windows：`build\Debug\tictactoe.exe`

### Makefile（Linux / macOS）

```bash
make
./build/tictactoe
```

---

## ビルドとテスト実行

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

期待される出力（全27テストが合格）：

```
Results: 27/27 passed
```

---

## プロジェクト構成

```
tictactoe-c/
├── src/
│   ├── game.h        公開 API（型・定数・関数宣言）
│   ├── game.c        コアゲームロジック（盤面・着手検証・勝利判定）
│   └── main.c        インタラクティブな2人対戦 CLI
├── tests/
│   └── test_game.c   27個のユニットテスト（組み込みのゼロ依存テストランナー）
├── .github/
│   └── workflows/
│       ├── ci.yml        CI — ビルド・テスト・静的解析・サニタイザー・valgrind
│       └── release.yml   CD — タグ時にバイナリをビルドして GitHub Release を公開
├── CMakeLists.txt    主要ビルドシステム
├── Makefile          Linux/macOS 用便利ビルド
└── ...
```

---

## API リファレンス

すべての公開シンボルは `src/game.h` で宣言されています。

| 関数 | 説明 | 戻り値 |
|------|------|--------|
| `game_init(n, k)` | n×n 盤面を初期化、k個連続で勝利 | `OK` / `FAIL` |
| `game_make_move(r, c, sym)` | (row, col) に X または O を配置 | `OK` / `FAIL` |
| `game_board_is_full()` | 引き分け条件の確認 | `1` / `0` |
| `game_check_horizontal(sym, k)` | 横方向の連続を走査 | `OK` / `FAIL` / `ERR` |
| `game_check_vertical(sym, k)` | 縦方向の連続を走査 | `OK` / `FAIL` / `ERR` |
| `game_check_diagonals(sym, k)` | ↘ 対角線を走査 | `OK` / `FAIL` / `ERR` |
| `game_check_anti_diagonals(sym, k)` | ↙ 逆対角線を走査 | `OK` / `FAIL` / `ERR` |
| `game_player_has_won(sym, k)` | 全盤面スキャンによる勝利判定（4方向） | `OK` / `FAIL` / `ERR` |
| `game_player_has_won_fast(r, c, sym, k)` | 最終着手を起点とした効率的な勝利判定 | `OK` / `FAIL` / `ERR` |
| `game_show_grid()` | 現在の盤面を stdout に表示 | — |

戻り値コード：`TICTACTOE_OK = 1`、`TICTACTOE_FAIL = 0`、`TICTACTOE_ERR = -1`

---

## アルゴリズム

`game_player_has_won` はすべての行・列・対角線をスキャンします — 1回の呼び出しで **O(n²)**。

`game_player_has_won_fast` は最後に置かれた駒を通る4方向のみを検査します：

```
←→  横方向
↕   縦方向
↘↖  対角線
↙↗  逆対角線
```

各方向で新しい駒から外側に向かって走査し、不一致が見つかった時点で終了するため、1回の呼び出しで **O(勝利条件長)** で動作します。これは人間のプレイヤーが自然に勝利を確認する方法と同じアプローチです。

---

## 実行例

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

## 貢献

[CONTRIBUTING.md](CONTRIBUTING.md) をご参照ください。

---

## ライセンス

[MIT](LICENSE) © 2026 frankwyf
