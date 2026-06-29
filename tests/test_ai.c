/*
 * test_ai.c — Unit tests for the AI opponent
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#include <stdio.h>
#include <stdlib.h>
#include "../src/game.h"
#include "../src/ai.h"

/* ── Minimal test framework (same as test_game.c) ──────────────────────── */

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

#define ASSERT_EQ(expected, actual, msg) ASSERT((expected) == (actual), msg)
#define RUN_TEST(fn) do { printf("\n[TEST] " #fn "\n"); fn(); } while (0)

/* ── Tests ─────────────────────────────────────────────────────────────── */

static void test_ai_invalid_params(void)
{
    int r, c;
    game_init(3, 3);
    ASSERT_EQ(TICTACTOE_ERR, ai_get_move('Z', AI_HARD, &r, &c), "bad symbol");
    ASSERT_EQ(TICTACTOE_ERR, ai_get_move(PLAYER_X, 99, &r, &c), "bad difficulty");
    ASSERT_EQ(TICTACTOE_ERR, ai_get_move(PLAYER_X, AI_HARD, NULL, &c), "null out_row");
    ASSERT_EQ(TICTACTOE_ERR, ai_get_move(PLAYER_X, AI_HARD, &r, NULL), "null out_col");
}

static void test_ai_easy_returns_valid_move(void)
{
    game_init(3, 3);
    int r, c;
    int ret = ai_get_move(PLAYER_X, AI_EASY, &r, &c);
    ASSERT_EQ(TICTACTOE_OK, ret, "easy: returns OK on empty board");
    ASSERT(r >= 0 && r < 3, "easy: row in bounds");
    ASSERT(c >= 0 && c < 3, "easy: col in bounds");
    ASSERT_EQ(CELL_EMPTY, grid[r][c], "easy: chosen cell is empty");
}

static void test_ai_medium_wins_immediately(void)
{
    /* O can win by placing at (2,2) */
    game_init(3, 3);
    grid[0][0] = PLAYER_X; grid[0][1] = PLAYER_X;
    grid[1][0] = PLAYER_O; grid[1][1] = PLAYER_O;
    /* O has (1,0) and (1,1) — winning move is (1,2) */
    int r, c;
    ai_get_move(PLAYER_O, AI_MEDIUM, &r, &c);
    ASSERT(r == 1 && c == 2, "medium: takes immediate win at (1,2)");
}

static void test_ai_medium_blocks_opponent(void)
{
    /* X has (0,0) and (0,1), about to win at (0,2).  O should block. */
    game_init(3, 3);
    grid[0][0] = PLAYER_X; grid[0][1] = PLAYER_X;
    grid[1][0] = PLAYER_O;
    int r, c;
    ai_get_move(PLAYER_O, AI_MEDIUM, &r, &c);
    ASSERT(r == 0 && c == 2, "medium: blocks X win at (0,2)");
}

static void test_ai_hard_wins_immediately(void)
{
    /* Same as medium test — hard should also take instant win */
    game_init(3, 3);
    grid[1][0] = PLAYER_O; grid[1][1] = PLAYER_O;
    grid[0][0] = PLAYER_X; grid[0][1] = PLAYER_X;
    grid[2][2] = PLAYER_X;
    int r, c;
    ai_get_move(PLAYER_O, AI_HARD, &r, &c);
    ASSERT(r == 1 && c == 2, "hard: takes winning move (1,2)");
}

static void test_ai_hard_blocks_opponent(void)
{
    /* X has (0,0) and (0,1). O must block (0,2). */
    game_init(3, 3);
    grid[0][0] = PLAYER_X; grid[0][1] = PLAYER_X;
    grid[1][1] = PLAYER_O;
    int r, c;
    ai_get_move(PLAYER_O, AI_HARD, &r, &c);
    ASSERT(r == 0 && c == 2, "hard: blocks X win at (0,2)");
}

static void test_ai_hard_never_loses_3x3(void)
{
    /*
     * Play a full 3×3 game: Human (X) plays random; AI (O) plays hard.
     * AI should never lose. Run several games.
     */
    int ai_losses = 0;
    for (int trial = 0; trial < 20; trial++) {
        game_init(3, 3);
        char current = PLAYER_X;
        int winner = 0;

        while (!game_board_is_full() && !winner) {
            int r, c;
            if (current == PLAYER_O) {
                ai_get_move(PLAYER_O, AI_HARD, &r, &c);
            } else {
                /* X plays first empty cell (deterministic "bad" strategy) */
                for (r = 0; r < 3; r++) {
                    int found = 0;
                    for (c = 0; c < 3; c++)
                        if (grid[r][c] == CELL_EMPTY) { found = 1; break; }
                    if (found) break;
                }
            }
            game_make_move(r, c, current);
            if (game_player_has_won_fast(r, c, current, 3) == TICTACTOE_OK)
                winner = current;
            current = (current == PLAYER_X) ? PLAYER_O : PLAYER_X;
        }
        if (winner == PLAYER_X) ai_losses++;
    }
    ASSERT_EQ(0, ai_losses, "hard: AI(O) never loses in 20 games vs naive X");
}

static void test_ai_full_board_returns_fail(void)
{
    /* Fill the board completely */
    game_init(3, 3);
    const char fill[] = "XOXXOOOXX";
    for (int i = 0; i < 9; i++)
        grid[i / 3][i % 3] = fill[i];

    int r, c;
    int ret = ai_get_move(PLAYER_O, AI_EASY, &r, &c);
    ASSERT_EQ(TICTACTOE_FAIL, ret, "full board: returns FAIL");
}

/* ── Entry point ─────────────────────────────────────────────────────────── */

int main(void)
{
    printf("Running AI test suite\n");
    printf("=======================\n");

    RUN_TEST(test_ai_invalid_params);
    RUN_TEST(test_ai_easy_returns_valid_move);
    RUN_TEST(test_ai_medium_wins_immediately);
    RUN_TEST(test_ai_medium_blocks_opponent);
    RUN_TEST(test_ai_hard_wins_immediately);
    RUN_TEST(test_ai_hard_blocks_opponent);
    RUN_TEST(test_ai_hard_never_loses_3x3);
    RUN_TEST(test_ai_full_board_returns_fail);

    printf("\n=======================\n");
    printf("Results: %d/%d passed", g_tests_passed, g_tests_run);
    if (g_tests_failed > 0)
        printf("  (%d FAILED)", g_tests_failed);
    printf("\n");

    return g_tests_failed == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
