/*
 * bench/bench_win_check.c — Benchmark: full-scan vs fast win-check
 *
 * Measures call counts and wall-clock time to demonstrate the O(n²) vs
 * O(win_length) performance difference.
 *
 * Build:
 *   gcc -std=c11 -O2 -I ../src ../src/game.c bench_win_check.c -o bench_win_check
 *
 * MIT License — see ../LICENSE
 * Copyright (c) 2026 frankwyf
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"

#define ITERATIONS 100000

static double time_full_scan(char symbol, int length, int iters)
{
    clock_t start = clock();
    volatile int result = 0;
    for (int i = 0; i < iters; i++)
        result = game_player_has_won(symbol, length);
    clock_t end = clock();
    (void)result;
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static double time_fast_check(int row, int col, char symbol, int length, int iters)
{
    clock_t start = clock();
    volatile int result = 0;
    for (int i = 0; i < iters; i++)
        result = game_player_has_won_fast(row, col, symbol, length);
    clock_t end = clock();
    (void)result;
    return (double)(end - start) / CLOCKS_PER_SEC;
}

static void run_bench(int gridsize, int winlength)
{
    game_init(gridsize, winlength);

    /* Place a winning horizontal line at the bottom row */
    int last_row = gridsize - 1;
    for (int c = 0; c < winlength; c++)
        grid[last_row][c] = PLAYER_X;

    int last_col = winlength - 1;

    printf("  Grid: %2d×%-2d  Win: %d  Iters: %d\n",
           gridsize, gridsize, winlength, ITERATIONS);

    double t_full = time_full_scan(PLAYER_X, winlength, ITERATIONS);
    double t_fast = time_fast_check(last_row, last_col, PLAYER_X, winlength, ITERATIONS);

    printf("    full-scan:  %.4f s\n", t_full);
    printf("    fast-check: %.4f s\n", t_fast);
    if (t_fast > 0.0)
        printf("    speedup:    %.1fx\n", t_full / t_fast);
    printf("\n");
}

int main(void)
{
    printf("=== Win-Check Performance Benchmark ===\n");
    printf("    (%d iterations per measurement)\n\n", ITERATIONS);

    run_bench(3, 3);
    run_bench(5, 4);
    run_bench(7, 5);
    run_bench(10, 5);
    run_bench(10, 10);

    printf("Done.\n");
    return EXIT_SUCCESS;
}
