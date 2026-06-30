/*
 * main.c — Interactive Tic-Tac-Toe CLI (Human vs Human / Human vs AI)
 *
 * MIT License — see LICENSE for details
 * Copyright (c) 2026 frankwyf
 */
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "ai.h"
#ifdef TICTACTOE_VERSION
#include "version.h"
#endif

#define MAX_MOVES (TICTACTOE_MAX_GRID * TICTACTOE_MAX_GRID)

typedef struct {
    int  row;
    int  col;
    char symbol;
} Move;

/* Discard the rest of the current input line. */
static void flush_stdin(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* Prompt for an integer in [min_val, max_val], re-prompting on bad input. */
static int read_int(const char *prompt, int min_val, int max_val)
{
    int val;
    for (;;) {
        printf("%s", prompt);
        fflush(stdout);
        if (scanf("%d", &val) == 1 && val >= min_val && val <= max_val) {
            flush_stdin();
            return val;
        }
        flush_stdin();
        printf("Please enter a number between %d and %d.\n", min_val, max_val);
    }
}

int main(void)
{
    Move history[MAX_MOVES];
    int  move_count = 0;

    printf("=== Tic-Tac-Toe ===\n");
#ifdef TICTACTOE_VERSION
    printf("    version %s\n", TICTACTOE_VERSION);
#endif
    printf("\n");

    /* ── Game mode selection ─────────────────────────────────────────────── */
    printf("Game modes:\n");
    printf("  1. Human vs Human\n");
    printf("  2. Human vs AI\n");
    int mode = read_int("Select mode (1-2): ", 1, 2);

    AIDifficulty ai_diff = AI_HARD;
    char ai_symbol = PLAYER_O;
    if (mode == 2) {
        printf("\nAI difficulty:\n");
        printf("  1. Easy   (random)\n");
        printf("  2. Medium (heuristic)\n");
        printf("  3. Hard   (minimax)\n");
        ai_diff = (AIDifficulty)read_int("Select difficulty (1-3): ", 1, 3);
        ai_symbol = PLAYER_O;
        printf("\nYou play as X, AI plays as O.\n");
    }

    /* ── Game setup ──────────────────────────────────────────────────────── */
    int gridsize = read_int("\nEnter grid size   (3-10): ",
                            TICTACTOE_MIN_GRID, TICTACTOE_MAX_GRID);

    char winprompt[64];
    snprintf(winprompt, sizeof(winprompt),
             "Enter winning length (3-%d): ", gridsize);
    int winlength = read_int(winprompt, TICTACTOE_MIN_WIN, gridsize);

    if (!game_init(gridsize, winlength)) {
        fprintf(stderr, "Error: failed to initialize game.\n");
        return EXIT_FAILURE;
    }
    game_show_grid();

    /* ── Main game loop ──────────────────────────────────────────────────── */
    const char players[2] = {PLAYER_X, PLAYER_O};
    int  current = 0;
    char winner  = 0;

    while (!game_board_is_full()) {
        char symbol = players[current & 1];
        int row, col;

        if (mode == 2 && symbol == ai_symbol) {
            /* AI turn */
            if (ai_get_move(ai_symbol, ai_diff, &row, &col) != TICTACTOE_OK)
                break;
            game_make_move(row, col, symbol);
            history[move_count++] = (Move){row, col, symbol};
            printf("AI (%c) plays: (%d, %d)\n", symbol, row, col);
        } else {
            /* Human turn */
            for (;;) {
                printf("Player %c, Choose Location (row col): ", symbol);
                fflush(stdout);

                if (scanf("%d %d", &row, &col) != 2) {
                    flush_stdin();
                    printf("Invalid input — please enter two integers.\n");
                    continue;
                }
                flush_stdin();

                if (game_make_move(row, col, symbol) == TICTACTOE_OK) {
                    history[move_count++] = (Move){row, col, symbol};
                    break;
                }
                if (row < 0 || row >= g_grid_size || col < 0 || col >= g_grid_size)
                    printf("Index out of range, please re-enter\n");
                else
                    printf("This location is already taken\n");
            }
        }

        game_show_grid();

        if (game_player_has_won_fast(
                history[move_count - 1].row,
                history[move_count - 1].col,
                symbol, winlength) == TICTACTOE_OK) {
            winner = symbol;
            break;
        }
        current++;
    }

    /* ── End-of-game message ─────────────────────────────────────────────── */
    if (winner) {
        printf("**************************\n");
        printf("Player %c has won the game\n", winner);
        printf("**************************\n");
    } else {
        printf("Game over; there are no winners\n");
    }

    /* ── Replay ──────────────────────────────────────────────────────────── */
    printf("\nWould you like to play back the recorded game? (y/n): ");
    fflush(stdout);
    char ch;
    if (scanf(" %c", &ch) == 1 && (ch == 'y' || ch == 'Y')) {
        flush_stdin();
        game_init(gridsize, winlength);
        game_show_grid();

        for (int i = 0; i < move_count; i++) {
            printf("Next or Exit (n/e): ");
            fflush(stdout);
            if (scanf(" %c", &ch) != 1 || ch == 'e' || ch == 'E') {
                flush_stdin();
                break;
            }
            flush_stdin();
            game_make_move(history[i].row, history[i].col, history[i].symbol);
            game_show_grid();
        }
    } else {
        flush_stdin();
    }

    return EXIT_SUCCESS;
}
