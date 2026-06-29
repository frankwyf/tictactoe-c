/*
 * game.c — Core game logic for Generalized Tic-Tac-Toe
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#include <stdio.h>
#include "game.h"

/* ── Module-level state ────────────────────────────────────────────────── */

char grid[TICTACTOE_MAX_GRID][TICTACTOE_MAX_GRID];
int  g_grid_size;
int  g_win_length;

/* ── Internal helpers ──────────────────────────────────────────────────── */

static int is_valid_symbol(char s) { return s == PLAYER_X || s == PLAYER_O; }
static int is_valid_length(int l)  { return l >= TICTACTOE_MIN_WIN && l <= g_grid_size; }

/* ── Public API ────────────────────────────────────────────────────────── */

int game_init(int grid_size, int win_length)
{
    if (grid_size < TICTACTOE_MIN_GRID || grid_size > TICTACTOE_MAX_GRID)
        return TICTACTOE_FAIL;
    if (win_length < TICTACTOE_MIN_WIN || win_length > grid_size)
        return TICTACTOE_FAIL;

    g_grid_size  = grid_size;
    g_win_length = win_length;

    for (int r = 0; r < g_grid_size; r++)
        for (int c = 0; c < g_grid_size; c++)
            grid[r][c] = CELL_EMPTY;

    return TICTACTOE_OK;
}

void game_show_grid(void)
{
    printf("\n\t");
    for (int c = 0; c < g_grid_size; c++)
        printf("%d ", c);
    printf("\n");
    for (int r = 0; r < g_grid_size; r++) {
        printf("%d\t", r);
        for (int c = 0; c < g_grid_size; c++)
            printf("%c ", grid[r][c]);
        printf("\n");
    }
    printf("\n");
}

int game_make_move(int row, int col, char symbol)
{
    if (!is_valid_symbol(symbol))           return TICTACTOE_FAIL;
    if (row < 0 || row >= g_grid_size)      return TICTACTOE_FAIL;
    if (col < 0 || col >= g_grid_size)      return TICTACTOE_FAIL;
    if (grid[row][col] != CELL_EMPTY)       return TICTACTOE_FAIL;
    grid[row][col] = symbol;
    return TICTACTOE_OK;
}

int game_board_is_full(void)
{
    for (int r = 0; r < g_grid_size; r++)
        for (int c = 0; c < g_grid_size; c++)
            if (grid[r][c] == CELL_EMPTY) return 0;
    return 1;
}

int game_check_horizontal(char symbol, int length)
{
    if (!is_valid_symbol(symbol)) return TICTACTOE_ERR;
    if (!is_valid_length(length)) return TICTACTOE_ERR;

    for (int r = 0; r < g_grid_size; r++) {
        int run = 0;
        for (int c = 0; c < g_grid_size; c++) {
            run = (grid[r][c] == symbol) ? run + 1 : 0;
            if (run >= length) return TICTACTOE_OK;
        }
    }
    return TICTACTOE_FAIL;
}

int game_check_vertical(char symbol, int length)
{
    if (!is_valid_symbol(symbol)) return TICTACTOE_ERR;
    if (!is_valid_length(length)) return TICTACTOE_ERR;

    for (int c = 0; c < g_grid_size; c++) {
        int run = 0;
        for (int r = 0; r < g_grid_size; r++) {
            run = (grid[r][c] == symbol) ? run + 1 : 0;
            if (run >= length) return TICTACTOE_OK;
        }
    }
    return TICTACTOE_FAIL;
}

int game_check_diagonals(char symbol, int length)
{
    if (!is_valid_symbol(symbol)) return TICTACTOE_ERR;
    if (!is_valid_length(length)) return TICTACTOE_ERR;

    /* d = col - row is constant along each top-left→bottom-right diagonal */
    for (int d = -(g_grid_size - 1); d <= g_grid_size - 1; d++) {
        int run = 0;
        for (int r = 0; r < g_grid_size; r++) {
            int c = r + d;
            if (c < 0 || c >= g_grid_size) { run = 0; continue; }
            run = (grid[r][c] == symbol) ? run + 1 : 0;
            if (run >= length) return TICTACTOE_OK;
        }
    }
    return TICTACTOE_FAIL;
}

int game_check_anti_diagonals(char symbol, int length)
{
    if (!is_valid_symbol(symbol)) return TICTACTOE_ERR;
    if (!is_valid_length(length)) return TICTACTOE_ERR;

    /* s = row + col is constant along each top-right→bottom-left anti-diagonal */
    for (int s = 0; s <= 2 * (g_grid_size - 1); s++) {
        int run = 0;
        for (int r = 0; r < g_grid_size; r++) {
            int c = s - r;
            if (c < 0 || c >= g_grid_size) { run = 0; continue; }
            run = (grid[r][c] == symbol) ? run + 1 : 0;
            if (run >= length) return TICTACTOE_OK;
        }
    }
    return TICTACTOE_FAIL;
}

int game_player_has_won(char symbol, int length)
{
    if (!is_valid_symbol(symbol)) return TICTACTOE_ERR;
    if (!is_valid_length(length)) return TICTACTOE_ERR;

    if (game_check_horizontal(symbol, length)      == TICTACTOE_OK) return TICTACTOE_OK;
    if (game_check_vertical(symbol, length)        == TICTACTOE_OK) return TICTACTOE_OK;
    if (game_check_diagonals(symbol, length)       == TICTACTOE_OK) return TICTACTOE_OK;
    if (game_check_anti_diagonals(symbol, length)  == TICTACTOE_OK) return TICTACTOE_OK;
    return TICTACTOE_FAIL;
}

int game_player_has_won_fast(int row, int col, char symbol, int length)
{
    if (!is_valid_symbol(symbol))               return TICTACTOE_ERR;
    if (!is_valid_length(length))               return TICTACTOE_ERR;
    if (row < 0 || row >= g_grid_size)          return TICTACTOE_ERR;
    if (col < 0 || col >= g_grid_size)          return TICTACTOE_ERR;

    /*
     * Walk in direction (dr, dc) and the opposite, counting consecutive
     * matches.  Starting count is 1 to include the cell at (row, col).
     */
#define COUNT_RUN(dr, dc)                                                        \
    do {                                                                         \
        int _run = 1, _r, _c;                                                    \
        for (_r = (row) + (dr), _c = (col) + (dc);                              \
             _r >= 0 && _r < g_grid_size && _c >= 0 && _c < g_grid_size         \
             && grid[_r][_c] == (symbol);                                        \
             _r += (dr), _c += (dc)) _run++;                                     \
        for (_r = (row) - (dr), _c = (col) - (dc);                              \
             _r >= 0 && _r < g_grid_size && _c >= 0 && _c < g_grid_size         \
             && grid[_r][_c] == (symbol);                                        \
             _r -= (dr), _c -= (dc)) _run++;                                     \
        if (_run >= (length)) return TICTACTOE_OK;                               \
    } while (0)

    COUNT_RUN(0,  1);  /* horizontal    ←→  */
    COUNT_RUN(1,  0);  /* vertical       ↕  */
    COUNT_RUN(1,  1);  /* diagonal      ↘↖  */
    COUNT_RUN(1, -1);  /* anti-diagonal ↙↗  */

#undef COUNT_RUN
    return TICTACTOE_FAIL;
}
