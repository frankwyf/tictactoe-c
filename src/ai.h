/*
 * ai.h — AI opponent for Generalized Tic-Tac-Toe
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#ifndef TICTACTOE_AI_H
#define TICTACTOE_AI_H

#include "game.h"

/** Difficulty levels for the AI. */
typedef enum {
    AI_EASY   = 1,   /**< Random valid moves                              */
    AI_MEDIUM = 2,   /**< Win-if-possible, else block, else random         */
    AI_HARD   = 3    /**< Minimax with alpha-beta pruning (optimal play)   */
} AIDifficulty;

/**
 * Compute the best move for the given symbol using the specified difficulty.
 *
 * @param symbol     PLAYER_X or PLAYER_O (the AI's symbol).
 * @param difficulty AI_EASY, AI_MEDIUM, or AI_HARD.
 * @param[out] out_row  Best row (only written on success).
 * @param[out] out_col  Best column (only written on success).
 * @return TICTACTOE_OK on success, TICTACTOE_FAIL if no move is possible,
 *         TICTACTOE_ERR on invalid parameters.
 */
int ai_get_move(char symbol, AIDifficulty difficulty, int *out_row, int *out_col);

#endif /* TICTACTOE_AI_H */
