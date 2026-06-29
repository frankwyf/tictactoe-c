/*
 * examples/sample_game.c — Demonstrates the tictactoe-c library API
 *
 * Runs a scripted 3×3 game programmatically (no interactive input).
 * Useful as a quick smoke-test and as a code-reading reference.
 *
 * Build:
 *   gcc -std=c11 -I../src ../src/game.c sample_game.c -o sample_game
 *
 * MIT License — see ../LICENSE
 * Copyright (c) 2026 frankwyf
 */
#include <stdio.h>
#include <stdlib.h>
#include "../src/game.h"

/* ── Helper ─────────────────────────────────────────────────────────────── */

static void make_move_checked(int row, int col, char symbol)
{
    int result = game_make_move(row, col, symbol);
    if (result != TICTACTOE_OK) {
        fprintf(stderr, "Unexpected invalid move at (%d,%d) for %c\n",
                row, col, symbol);
        exit(EXIT_FAILURE);
    }
    game_show_grid();
}

/* ── Main ────────────────────────────────────────────────────────────────── */

int main(void)
{
    /* ── Example 1: Classic 3×3, X wins diagonally ─────────────────────── */
    printf("=== Example 1: 3×3 game, win=3 ===\n");
    game_init(3, 3);
    game_show_grid();

    struct { int r, c; char s; } moves[] = {
        {0, 0, 'X'}, {0, 1, 'O'},
        {1, 1, 'X'}, {0, 2, 'O'},
        {2, 2, 'X'},             /* X completes main diagonal */
    };
    int n = (int)(sizeof(moves) / sizeof(moves[0]));

    for (int i = 0; i < n; i++) {
        printf("Player %c → (%d,%d)\n", moves[i].s, moves[i].r, moves[i].c);
        make_move_checked(moves[i].r, moves[i].c, moves[i].s);

        if (game_player_has_won_fast(moves[i].r, moves[i].c,
                                     moves[i].s, g_win_length) == TICTACTOE_OK) {
            printf("Player %c wins!\n\n", moves[i].s);
            break;
        }
    }

    /* ── Example 2: 5×5 game, win=4, O wins vertically ─────────────────── */
    printf("=== Example 2: 5×5 game, win=4 ===\n");
    game_init(5, 4);
    game_show_grid();

    struct { int r, c; char s; } moves2[] = {
        {0, 2, 'O'}, {0, 0, 'X'},
        {1, 2, 'O'}, {1, 0, 'X'},
        {2, 2, 'O'}, {2, 0, 'X'},
        {3, 2, 'O'},             /* O completes 4-in-a-column */
    };
    int n2 = (int)(sizeof(moves2) / sizeof(moves2[0]));

    for (int i = 0; i < n2; i++) {
        printf("Player %c → (%d,%d)\n", moves2[i].s, moves2[i].r, moves2[i].c);
        make_move_checked(moves2[i].r, moves2[i].c, moves2[i].s);

        if (game_player_has_won_fast(moves2[i].r, moves2[i].c,
                                     moves2[i].s, g_win_length) == TICTACTOE_OK) {
            printf("Player %c wins!\n\n", moves2[i].s);
            break;
        }
    }

    return EXIT_SUCCESS;
}
