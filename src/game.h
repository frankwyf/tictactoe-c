/*
 * game.h — Public API for Generalized Tic-Tac-Toe
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#ifndef TICTACTOE_GAME_H
#define TICTACTOE_GAME_H

/** Maximum supported grid dimension. */
#define TICTACTOE_MAX_GRID  10

/** Minimum supported grid dimension. */
#define TICTACTOE_MIN_GRID   3

/** Minimum winning run length. */
#define TICTACTOE_MIN_WIN    3

/** Symbol constants. */
#define PLAYER_X   'X'
#define PLAYER_O   'O'
#define CELL_EMPTY '.'

/** Return-code constants. */
#define TICTACTOE_OK     1   /**< Success / win found       */
#define TICTACTOE_FAIL   0   /**< No win / invalid move     */
#define TICTACTOE_ERR  (-1)  /**< Invalid parameters        */

/* Board state — accessible for rendering and testing. */
extern char grid[TICTACTOE_MAX_GRID][TICTACTOE_MAX_GRID];
extern int  g_grid_size;
extern int  g_win_length;

/**
 * Initialize (or reset) a new game.
 *
 * @param grid_size  Board dimension (TICTACTOE_MIN_GRID..TICTACTOE_MAX_GRID).
 * @param win_length Consecutive symbols needed to win (TICTACTOE_MIN_WIN..grid_size).
 * @return TICTACTOE_OK on success, TICTACTOE_FAIL on invalid parameters.
 */
int game_init(int grid_size, int win_length);

/**
 * Place a symbol on the board.
 *
 * @param row    Row index (0-based).
 * @param col    Column index (0-based).
 * @param symbol PLAYER_X or PLAYER_O.
 * @return TICTACTOE_OK on success, TICTACTOE_FAIL if the move is illegal.
 */
int game_make_move(int row, int col, char symbol);

/**
 * Check whether every cell is occupied.
 *
 * @return 1 if full, 0 otherwise.
 */
int game_board_is_full(void);

/**
 * Scan the entire board for a horizontal run of `length`.
 *
 * @return TICTACTOE_OK (win), TICTACTOE_FAIL (no win), or TICTACTOE_ERR (bad args).
 */
int game_check_horizontal(char symbol, int length);

/** @see game_check_horizontal — vertical direction. */
int game_check_vertical(char symbol, int length);

/** @see game_check_horizontal — top-left to bottom-right diagonals. */
int game_check_diagonals(char symbol, int length);

/** @see game_check_horizontal — top-right to bottom-left anti-diagonals. */
int game_check_anti_diagonals(char symbol, int length);

/**
 * Check all four directions for a win (delegates to the four functions above).
 *
 * @return TICTACTOE_OK (win), TICTACTOE_FAIL (no win), or TICTACTOE_ERR (bad args).
 */
int game_player_has_won(char symbol, int length);

/**
 * Efficient win check anchored at the last-placed cell.
 *
 * Only inspects the four lines that pass through (row, col), achieving
 * O(win_length) time per call instead of O(grid_size^2).
 *
 * @param row    Row of the last move.
 * @param col    Column of the last move.
 * @param symbol Symbol just placed.
 * @param length Win condition length.
 * @return TICTACTOE_OK (win), TICTACTOE_FAIL (no win), or TICTACTOE_ERR (bad args).
 */
int game_player_has_won_fast(int row, int col, char symbol, int length);

/** Print the current board state to stdout. */
void game_show_grid(void);

#endif /* TICTACTOE_GAME_H */
