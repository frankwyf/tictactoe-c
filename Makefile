CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -g

SRCDIR   := src
TESTDIR  := tests
BUILDDIR := build

GAME_OBJ    := $(BUILDDIR)/game.o
MAIN_OBJ    := $(BUILDDIR)/main.o
TARGET      := $(BUILDDIR)/tictactoe
TEST_TARGET := $(BUILDDIR)/test_game

.PHONY: all test clean help

all: $(TARGET)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

$(GAME_OBJ): $(SRCDIR)/game.c $(SRCDIR)/game.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(MAIN_OBJ): $(SRCDIR)/main.c $(SRCDIR)/game.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(GAME_OBJ) $(MAIN_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

$(TEST_TARGET): $(TESTDIR)/test_game.c $(SRCDIR)/game.c $(SRCDIR)/game.h | $(BUILDDIR)
	$(CC) $(CFLAGS) -I$(SRCDIR) $(TESTDIR)/test_game.c $(SRCDIR)/game.c -o $@

test: $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -rf $(BUILDDIR)

help:
	@echo "Targets:"
	@echo "  all   — build the game executable (default)"
	@echo "  test  — build and run unit tests"
	@echo "  clean — remove build/ directory"
