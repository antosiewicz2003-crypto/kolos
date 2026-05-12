# AGENTS.md

## Cursor Cloud specific instructions

This is a C11 image processing project using the DANTE automated testing framework. It implements PGM image loading, thresholding, and area segmentation.

### Build & Run

- **Primary build system:** GNU Make (use `make build`, `make run_unit_tests`, `make run_main`)
- CMake is also configured but requires GCC explicitly (`CC=gcc cmake ..`) since the compiler flags use GCC-specific options like `-fmax-errors=5` that Clang rejects
- The build output goes to `build-dir/`; use `make clean` to clear it

### Key Gotchas

- The `[]` operator must NOT be used in student code (`defs.c`/`defs.h`) — use pointer arithmetic instead (assignment requirement)
- `translator.c` bridges the student implementation (`defs.c`) with the DANTE test harness; it provides `main()` which delegates to `__wrap_main()` in `unit_test_v2.c`, and defines `__real_main()` for the test framework
- Binary test data files (`*.bin`) must be present in the working directory for tests to pass — regenerate them with `python3 generate_test_data.py` if missing
- The DANTE memory tracker (`rdebug`) wraps `malloc`/`free`/`calloc`/`realloc` and enforces per-test heap limits; allocations must be efficient (e.g., use `uint8_t` for visited arrays, avoid over-allocating)
- PGM binary format: `"P2"` (2 bytes) + width (4 bytes LE int) + height (4 bytes LE int) + max_val (1 byte, must be 255) + pixel data (height*width bytes)
