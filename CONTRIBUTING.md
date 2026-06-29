# Contributing to tictactoe-c

Thank you for your interest in contributing!  
This document covers all you need to get started.

> This project accepts contributions in English, Chinese (中文), and Japanese (日本語).  
> Maintainers will respond in the language you use.

---

## Table of Contents

- [Reporting Bugs](#reporting-bugs)
- [Requesting Features](#requesting-features)
- [Submitting Code Changes](#submitting-code-changes)
- [Code Style](#code-style)
- [Commit Messages](#commit-messages)
- [Running Tests Locally](#running-tests-locally)
- [License](#license)

---

## Reporting Bugs

1. Search [existing issues](https://github.com/frankwyf/tictactoe-c/issues) first.
2. Open a new issue with:
   - A clear, descriptive title.
   - Steps to reproduce (ideally a minimal failing test case).
   - Expected vs. actual behaviour.
   - Platform and compiler information (`gcc --version`, OS version).

---

## Requesting Features

Open an issue with the label **enhancement** and describe:
- The use case / motivation.
- Proposed API changes (if any).

---

## Submitting Code Changes

1. **Fork** the repository and create a branch:
   ```bash
   git checkout -b feat/your-feature-name
   ```

2. Write your code following the [Code Style](#code-style) guidelines below.

3. **Add or update tests** in `tests/test_game.c`.  
   Every new function must have at least one positive and one negative test.

4. Verify all tests pass locally (see [Running Tests Locally](#running-tests-locally)).

5. Open a **Pull Request** against `main`:
   - Describe *what* changed and *why*.
   - Reference related issues with `Closes #123`.

---

## Code Style

| Rule | Detail |
|------|--------|
| Standard | C11 (`-std=c11`, no extensions) |
| Indentation | **4 spaces** — no tabs |
| Line length | ≤ 100 characters |
| Braces | Allman style (opening brace on its own line for functions) |
| Naming | `snake_case` for functions and variables; `UPPER_CASE` for macros |
| Public API | Every public function declared in `game.h` must have a doc-comment |
| Function length | Aim for < 50 lines; split if necessary |

Run `cppcheck` before submitting:

```bash
cppcheck --std=c11 --enable=all --suppress=missingIncludeSystem src/
```

---

## Commit Messages

Follow [Conventional Commits](https://www.conventionalcommits.org/):

```
feat: add AI opponent using minimax
fix: correct off-by-one in anti-diagonal scan
test: cover 10×10 edge cases
docs: update Japanese README
refactor: extract direction-walking helper
```

---

## Running Tests Locally

```bash
# Build and run tests
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure

# With AddressSanitizer (Linux / macOS)
cmake -B build -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_C_FLAGS="-fsanitize=address,undefined"
cmake --build build
./build/test_game
```

---

## License

By contributing you agree that your changes are released under the project's
[MIT License](LICENSE).
