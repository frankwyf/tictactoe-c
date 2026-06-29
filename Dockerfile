# syntax=docker/dockerfile:1
#
# Multi-stage Dockerfile for tictactoe-c
#
# Usage:
#   docker build -t tictactoe .                # build release binary
#   docker build --target test -t tictactoe-test .  # run tests
#   docker run --rm -it tictactoe              # play the game

# ── Stage 1: Build & Test ──────────────────────────────────────────────────
FROM gcc:14 AS build

WORKDIR /app
COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build --parallel \
    && ctest --test-dir build --output-on-failure

# Separate test target for CI convenience
FROM build AS test
RUN echo "All tests passed."

# ── Stage 2: Minimal runtime image ────────────────────────────────────────
FROM debian:bookworm-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
        libc6 \
    && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/build/tictactoe /usr/local/bin/tictactoe

ENTRYPOINT ["tictactoe"]
