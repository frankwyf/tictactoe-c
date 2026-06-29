/*
 * ai.c — AI opponent implementation (Minimax with alpha-beta pruning)
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "ai.h"

/* ── Internal helpers ──────────────────────────────────────────────────── */

static char opponent_of(char s) { return s == PLAYER_X ? PLAYER_O : PLAYER_X; }

/**
 * Check if placing `symbol` at (r,c) would win.
 * Assumes (r,c) already holds `symbol`.
 */
static int is_winning_move(int r, int c, char symbol)
{
    return game_player_has_won_fast(r, c, symbol, g_win_length) == TICTACTOE_OK;
}

/* ── Random move (AI_EASY) ─────────────────────────────────────────────── */

static int random_move(int *out_row, int *out_col)
{
    int empty[TICTACTOE_MAX_GRID * TICTACTOE_MAX_GRID][2];
    int count = 0;

    for (int r = 0; r < g_grid_size; r++)
        for (int c = 0; c < g_grid_size; c++)
            if (grid[r][c] == CELL_EMPTY) {
                empty[count][0] = r;
                empty[count][1] = c;
                count++;
            }

    if (count == 0) return TICTACTOE_FAIL;

    int idx = rand() % count;
    *out_row = empty[idx][0];
    *out_col = empty[idx][1];
    return TICTACTOE_OK;
}

/* ── Heuristic move (AI_MEDIUM) ────────────────────────────────────────── */

static int medium_move(char symbol, int *out_row, int *out_col)
{
    char opp = opponent_of(symbol);

    /* 1. Win if possible */
    for (int r = 0; r < g_grid_size; r++)
        for (int c = 0; c < g_grid_size; c++)
            if (grid[r][c] == CELL_EMPTY) {
                grid[r][c] = symbol;
                if (is_winning_move(r, c, symbol)) {
                    grid[r][c] = CELL_EMPTY;
                    *out_row = r; *out_col = c;
                    return TICTACTOE_OK;
                }
                grid[r][c] = CELL_EMPTY;
            }

    /* 2. Block opponent win */
    for (int r = 0; r < g_grid_size; r++)
        for (int c = 0; c < g_grid_size; c++)
            if (grid[r][c] == CELL_EMPTY) {
                grid[r][c] = opp;
                if (is_winning_move(r, c, opp)) {
                    grid[r][c] = CELL_EMPTY;
                    *out_row = r; *out_col = c;
                    return TICTACTOE_OK;
                }
                grid[r][c] = CELL_EMPTY;
            }

    /* 3. Fallback: center if free, else random */
    int mid = g_grid_size / 2;
    if (grid[mid][mid] == CELL_EMPTY) {
        *out_row = mid; *out_col = mid;
        return TICTACTOE_OK;
    }

    return random_move(out_row, out_col);
}

/* ── Minimax with Alpha-Beta (AI_HARD) ─────────────────────────────────── */

/*
 * Depth limit heuristic to keep search tractable on larger boards.
 * For 3×3 it's effectively unlimited (9); for 10×10 capped at a
 * practical depth.
 */
static int max_depth(void)
{
    if (g_grid_size <= 3) return g_grid_size * g_grid_size;
    if (g_grid_size <= 5) return 8;
    return 5;
}

static int minimax(char ai_sym, char current, int depth, int alpha, int beta,
                   int last_r, int last_c)
{
    /* Terminal checks (only after at least one move on this recursion path) */
    if (last_r >= 0) {
        char last_sym = opponent_of(current); /* who just moved */
        if (is_winning_move(last_r, last_c, last_sym)) {
            return (last_sym == ai_sym) ? 100 + depth : -(100 + depth);
        }
    }
    if (game_board_is_full() || depth == 0) return 0; /* draw or horizon */

    int is_max = (current == ai_sym);
    int best = is_max ? INT_MIN : INT_MAX;

    for (int r = 0; r < g_grid_size; r++) {
        for (int c = 0; c < g_grid_size; c++) {
            if (grid[r][c] != CELL_EMPTY) continue;

            grid[r][c] = current;
            int score = minimax(ai_sym, opponent_of(current), depth - 1,
                                alpha, beta, r, c);
            grid[r][c] = CELL_EMPTY;

            if (is_max) {
                if (score > best) best = score;
                if (best > alpha) alpha = best;
            } else {
                if (score < best) best = score;
                if (best < beta) beta = best;
            }
            if (beta <= alpha) goto cutoff;
        }
    }
cutoff:
    return best;
}

static int hard_move(char symbol, int *out_row, int *out_col)
{
    int best_score = INT_MIN;
    int best_r = -1, best_c = -1;
    int depth = max_depth();

    for (int r = 0; r < g_grid_size; r++) {
        for (int c = 0; c < g_grid_size; c++) {
            if (grid[r][c] != CELL_EMPTY) continue;

            grid[r][c] = symbol;
            int score = minimax(symbol, opponent_of(symbol), depth - 1,
                                INT_MIN, INT_MAX, r, c);
            grid[r][c] = CELL_EMPTY;

            if (score > best_score) {
                best_score = score;
                best_r = r;
                best_c = c;
            }
        }
    }

    if (best_r < 0) return TICTACTOE_FAIL;

    *out_row = best_r;
    *out_col = best_c;
    return TICTACTOE_OK;
}

/* ── Public API ────────────────────────────────────────────────────────── */

int ai_get_move(char symbol, AIDifficulty difficulty, int *out_row, int *out_col)
{
    if (!out_row || !out_col) return TICTACTOE_ERR;
    if (symbol != PLAYER_X && symbol != PLAYER_O) return TICTACTOE_ERR;
    if (difficulty < AI_EASY || difficulty > AI_HARD) return TICTACTOE_ERR;

    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }

    switch (difficulty) {
    case AI_EASY:   return random_move(out_row, out_col);
    case AI_MEDIUM: return medium_move(symbol, out_row, out_col);
    case AI_HARD:   return hard_move(symbol, out_row, out_col);
    }
    return TICTACTOE_ERR; /* unreachable */
}
