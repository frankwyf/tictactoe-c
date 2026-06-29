/*
 * test_game.c — Unit tests for the Tic-Tac-Toe game library
 *
 * Uses a self-contained, zero-dependency test framework.
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#include <stdio.h>
#include <stdlib.h>
#include "../src/game.h"

/* ── Minimal test framework ────────────────────────────────────────────── */

static int g_tests_run    = 0;
static int g_tests_passed = 0;
static int g_tests_failed = 0;

#define ASSERT(cond, msg)                                                        \
    do {                                                                         \
        g_tests_run++;                                                           \
        if (cond) {                                                              \
            printf("  [PASS] %s\n", (msg));                                      \
            g_tests_passed++;                                                    \
        } else {                                                                 \
            printf("  [FAIL] %s  (line %d)\n", (msg), __LINE__);                \
            g_tests_failed++;                                                    \
        }                                                                        \
    } while (0)

#define ASSERT_EQ(expected, actual, msg) \
    ASSERT((expected) == (actual), msg)

#define RUN_TEST(fn)  do { printf("\n[TEST] " #fn "\n"); fn(); } while (0)

/* ── Helper ─────────────────────────────────────────────────────────────── */

/** Place multiple moves from a flat array {row, col, symbol, ...}. */
static void place_moves(const int moves[][3], int count)
{
    for (int i = 0; i < count; i++)
        game_make_move(moves[i][0], moves[i][1], (char)moves[i][2]);
}

/* ── Test cases ──────────────────────────────────────────────────────────── */

static void test_init_valid(void)
{
    ASSERT_EQ(TICTACTOE_OK,   game_init(3, 3),  "3x3, win=3 succeeds");
    ASSERT_EQ(3, g_grid_size,                    "grid_size == 3");
    ASSERT_EQ(3, g_win_length,                   "win_length == 3");
    ASSERT_EQ(TICTACTOE_OK,   game_init(10, 5), "10x10, win=5 succeeds");
    ASSERT_EQ(TICTACTOE_OK,   game_init(5, 5),  "5x5, win=5 succeeds");
    ASSERT_EQ(TICTACTOE_OK,   game_init(3, 3),  "minimum valid params");
}

static void test_init_invalid(void)
{
    ASSERT_EQ(TICTACTOE_FAIL, game_init(2,  3), "gridsize < 3 fails");
    ASSERT_EQ(TICTACTOE_FAIL, game_init(11, 3), "gridsize > 10 fails");
    ASSERT_EQ(TICTACTOE_FAIL, game_init(3,  2), "winlength < 3 fails");
    ASSERT_EQ(TICTACTOE_FAIL, game_init(3,  4), "winlength > gridsize fails");
    ASSERT_EQ(TICTACTOE_FAIL, game_init(0,  0), "all-zero params fail");
}

static void test_init_clears_grid(void)
{
    game_init(5, 3);
    game_make_move(2, 2, PLAYER_X);
    game_init(5, 3); /* re-init should wipe the board */
    for (int r = 0; r < 5; r++)
        for (int c = 0; c < 5; c++)
            ASSERT_EQ(CELL_EMPTY, grid[r][c], "cell reset to CELL_EMPTY");
}

static void test_make_move_valid(void)
{
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_OK, game_make_move(0, 0, PLAYER_X), "X at (0,0)");
    ASSERT_EQ(PLAYER_X,     grid[0][0],                      "grid[0][0] == X");
    ASSERT_EQ(TICTACTOE_OK, game_make_move(2, 2, PLAYER_O), "O at (2,2)");
    ASSERT_EQ(PLAYER_O,     grid[2][2],                      "grid[2][2] == O");
}

static void test_make_move_invalid(void)
{
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_FAIL, game_make_move(-1,  0, PLAYER_X), "row -1 rejected");
    ASSERT_EQ(TICTACTOE_FAIL, game_make_move( 3,  0, PLAYER_X), "row 3 rejected (3x3)");
    ASSERT_EQ(TICTACTOE_FAIL, game_make_move( 0, -1, PLAYER_X), "col -1 rejected");
    ASSERT_EQ(TICTACTOE_FAIL, game_make_move( 0,  3, PLAYER_X), "col 3 rejected (3x3)");
    ASSERT_EQ(TICTACTOE_FAIL, game_make_move( 0,  0, 'A'),      "invalid symbol rejected");
    game_make_move(1, 1, PLAYER_X);
    ASSERT_EQ(TICTACTOE_FAIL, game_make_move(1,  1, PLAYER_O), "occupied cell rejected");
}

static void test_board_is_full(void)
{
    game_init(3, 3);
    ASSERT_EQ(0, game_board_is_full(), "empty board is not full");

    /* Fill with a draw pattern: X O X / O X O / O X O */
    const int moves[][3] = {
        {0,0,'X'},{0,1,'O'},{0,2,'X'},
        {1,0,'O'},{1,1,'X'},{1,2,'O'},
        {2,0,'O'},{2,1,'X'},{2,2,'O'}
    };
    place_moves(moves, 9);
    ASSERT_EQ(1, game_board_is_full(), "filled board is full");
}

static void test_check_horizontal_win(void)
{
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_FAIL, game_check_horizontal(PLAYER_X, 3), "no win initially");
    game_make_move(0, 0, PLAYER_X);
    game_make_move(0, 1, PLAYER_X);
    game_make_move(0, 2, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK,   game_check_horizontal(PLAYER_X, 3), "row-0 win for X");
    ASSERT_EQ(TICTACTOE_FAIL, game_check_horizontal(PLAYER_O, 3), "no win for O");
}

static void test_check_horizontal_middle(void)
{
    game_init(5, 3);
    game_make_move(2, 1, PLAYER_X);
    game_make_move(2, 2, PLAYER_X);
    game_make_move(2, 3, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_check_horizontal(PLAYER_X, 3), "mid-board horizontal win");
}

static void test_check_horizontal_invalid_args(void)
{
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_ERR, game_check_horizontal('Z', 3), "bad symbol → ERR");
    ASSERT_EQ(TICTACTOE_ERR, game_check_horizontal(PLAYER_X, 2), "length < 3 → ERR");
    ASSERT_EQ(TICTACTOE_ERR, game_check_horizontal(PLAYER_X, 4), "length > gridsize → ERR");
}

static void test_check_vertical_win(void)
{
    game_init(3, 3);
    game_make_move(0, 2, PLAYER_O);
    game_make_move(1, 2, PLAYER_O);
    game_make_move(2, 2, PLAYER_O);
    ASSERT_EQ(TICTACTOE_OK,   game_check_vertical(PLAYER_O, 3), "col-2 win for O");
    ASSERT_EQ(TICTACTOE_FAIL, game_check_vertical(PLAYER_X, 3), "no vertical win for X");
}

static void test_check_diagonal_main(void)
{
    game_init(3, 3);
    game_make_move(0, 0, PLAYER_X);
    game_make_move(1, 1, PLAYER_X);
    game_make_move(2, 2, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_check_diagonals(PLAYER_X, 3), "main diagonal win");
}

static void test_check_diagonal_offset(void)
{
    /* Diagonal offset by 1: (1,0)→(2,1)→(3,2) on a 5×5 board */
    game_init(5, 3);
    game_make_move(1, 0, PLAYER_O);
    game_make_move(2, 1, PLAYER_O);
    game_make_move(3, 2, PLAYER_O);
    ASSERT_EQ(TICTACTOE_OK, game_check_diagonals(PLAYER_O, 3), "offset diagonal win");
}

static void test_check_anti_diagonal_main(void)
{
    game_init(3, 3);
    game_make_move(0, 2, PLAYER_X);
    game_make_move(1, 1, PLAYER_X);
    game_make_move(2, 0, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_check_anti_diagonals(PLAYER_X, 3), "main anti-diagonal win");
}

static void test_check_anti_diagonal_offset(void)
{
    /* Anti-diagonal: (1,3)→(2,2)→(3,1) on a 5×5 board */
    game_init(5, 3);
    game_make_move(1, 3, PLAYER_O);
    game_make_move(2, 2, PLAYER_O);
    game_make_move(3, 1, PLAYER_O);
    ASSERT_EQ(TICTACTOE_OK, game_check_anti_diagonals(PLAYER_O, 3), "offset anti-diagonal win");
}

static void test_player_has_won_invalid(void)
{
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won('!', 3), "bad symbol → ERR");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won(PLAYER_X, 2), "length < 3 → ERR");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won(PLAYER_X, 4), "length > gridsize → ERR");
}

static void test_player_has_won_no_win(void)
{
    game_init(3, 3);
    game_make_move(0, 0, PLAYER_X);
    game_make_move(1, 1, PLAYER_O);
    ASSERT_EQ(TICTACTOE_FAIL, game_player_has_won(PLAYER_X, 3), "no winner yet");
}

static void test_player_has_won_horizontal(void)
{
    game_init(3, 3);
    game_make_move(2, 0, PLAYER_X);
    game_make_move(2, 1, PLAYER_X);
    game_make_move(2, 2, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won(PLAYER_X, 3), "horizontal win detected");
}

static void test_fast_win_horizontal(void)
{
    game_init(3, 3);
    game_make_move(0, 0, PLAYER_X);
    game_make_move(0, 1, PLAYER_X);
    game_make_move(0, 2, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won_fast(0, 2, PLAYER_X, 3), "fast: horizontal");
}

static void test_fast_win_vertical(void)
{
    game_init(3, 3);
    game_make_move(0, 1, PLAYER_O);
    game_make_move(1, 1, PLAYER_O);
    game_make_move(2, 1, PLAYER_O);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won_fast(2, 1, PLAYER_O, 3), "fast: vertical");
}

static void test_fast_win_diagonal(void)
{
    /* (1,0)→(2,1)→(3,2) on 4×4 grid */
    game_init(4, 3);
    game_make_move(1, 0, PLAYER_X);
    game_make_move(2, 1, PLAYER_X);
    game_make_move(3, 2, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won_fast(3, 2, PLAYER_X, 3), "fast: diagonal");
}

static void test_fast_win_anti_diagonal(void)
{
    /* (1,3)→(2,2)→(3,1) on 4×4 grid */
    game_init(4, 3);
    game_make_move(1, 3, PLAYER_O);
    game_make_move(2, 2, PLAYER_O);
    game_make_move(3, 1, PLAYER_O);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won_fast(3, 1, PLAYER_O, 3), "fast: anti-diagonal");
}

static void test_fast_win_no_win(void)
{
    game_init(3, 3);
    game_make_move(0, 0, PLAYER_X);
    game_make_move(1, 1, PLAYER_O);
    ASSERT_EQ(TICTACTOE_FAIL, game_player_has_won_fast(0, 0, PLAYER_X, 3), "fast: no win");
}

static void test_fast_win_invalid(void)
{
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won_fast( 0,  0, '$', 3), "fast: bad symbol");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won_fast(-1,  0, PLAYER_X, 3), "fast: row < 0");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won_fast( 3,  0, PLAYER_X, 3), "fast: row ≥ n");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won_fast( 0, -1, PLAYER_X, 3), "fast: col < 0");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won_fast( 0,  3, PLAYER_X, 3), "fast: col ≥ n");
    ASSERT_EQ(TICTACTOE_ERR, game_player_has_won_fast( 0,  0, PLAYER_X, 2), "fast: length < 3");
}

static void test_larger_grid_4_in_row(void)
{
    game_init(5, 4);
    game_make_move(2, 0, PLAYER_X);
    game_make_move(2, 1, PLAYER_X);
    game_make_move(2, 2, PLAYER_X);
    game_make_move(2, 3, PLAYER_X);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won(PLAYER_X, 4),               "5×5: 4-in-row scan");
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won_fast(2, 3, PLAYER_X, 4),   "5×5: 4-in-row fast");
}

static void test_large_grid_vertical_5(void)
{
    game_init(10, 5);
    for (int r = 0; r < 5; r++)
        game_make_move(r, 7, PLAYER_O);
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won(PLAYER_O, 5),              "10×10: 5-in-col scan");
    ASSERT_EQ(TICTACTOE_OK, game_player_has_won_fast(4, 7, PLAYER_O, 5),  "10×10: 5-in-col fast");
}

static void test_no_false_win_blocked_row(void)
{
    /* X O X — should NOT be a win */
    game_init(3, 3);
    game_make_move(0, 0, PLAYER_X);
    game_make_move(0, 1, PLAYER_O);
    game_make_move(0, 2, PLAYER_X);
    ASSERT_EQ(TICTACTOE_FAIL, game_check_horizontal(PLAYER_X, 3), "blocked row not a win");
}

static void test_consistency_scan_vs_fast(void)
{
    /* Verify fast and full-scan agree on the same board */
    game_init(4, 3);
    const int moves[][3] = {
        {0,0,'X'},{0,1,'O'},{1,0,'X'},
        {1,1,'O'},{2,0,'X'}
    };
    place_moves(moves, 5);
    int scan = game_player_has_won(PLAYER_X, 3);
    int fast = game_player_has_won_fast(2, 0, PLAYER_X, 3);
    ASSERT(scan == fast, "scan and fast agree on winning position");
}

/* ── Entry point ─────────────────────────────────────────────────────────── */

int main(void)
{
    printf("Running Tic-Tac-Toe test suite\n");
    printf("================================\n");

    RUN_TEST(test_init_valid);
    RUN_TEST(test_init_invalid);
    RUN_TEST(test_init_clears_grid);
    RUN_TEST(test_make_move_valid);
    RUN_TEST(test_make_move_invalid);
    RUN_TEST(test_board_is_full);
    RUN_TEST(test_check_horizontal_win);
    RUN_TEST(test_check_horizontal_middle);
    RUN_TEST(test_check_horizontal_invalid_args);
    RUN_TEST(test_check_vertical_win);
    RUN_TEST(test_check_diagonal_main);
    RUN_TEST(test_check_diagonal_offset);
    RUN_TEST(test_check_anti_diagonal_main);
    RUN_TEST(test_check_anti_diagonal_offset);
    RUN_TEST(test_player_has_won_invalid);
    RUN_TEST(test_player_has_won_no_win);
    RUN_TEST(test_player_has_won_horizontal);
    RUN_TEST(test_fast_win_horizontal);
    RUN_TEST(test_fast_win_vertical);
    RUN_TEST(test_fast_win_diagonal);
    RUN_TEST(test_fast_win_anti_diagonal);
    RUN_TEST(test_fast_win_no_win);
    RUN_TEST(test_fast_win_invalid);
    RUN_TEST(test_larger_grid_4_in_row);
    RUN_TEST(test_large_grid_vertical_5);
    RUN_TEST(test_no_false_win_blocked_row);
    RUN_TEST(test_consistency_scan_vs_fast);

    printf("\n================================\n");
    printf("Results: %d/%d passed", g_tests_passed, g_tests_run);
    if (g_tests_failed > 0)
        printf("  (%d FAILED)", g_tests_failed);
    printf("\n");

    return g_tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
