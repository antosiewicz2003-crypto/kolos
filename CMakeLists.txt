#
# DANTE :: System Automatycznych Testów Jednostkowych oraz Akceptacyjnych
# Tomasz Jaworski, 2018-2020
#
# Plik wygenerowany automatycznie
# Znacznik czasowy: 2026-05-12 11:42:53.367299
#

cmake_minimum_required(VERSION 3.17)
project(project1 C)

# Przyjmij standard C11
set(CMAKE_C_STANDARD 11)

# Ustaw opcje kompilatora (z raportu Dante)
add_compile_options(
      "-Werror=vla"
      "-D_GNU_SOURCE"
      "-DINSIDE_DANTE"
      "-xc"
      "-Wno-parentheses"
      "-Werror"
      "-std=c11"
      "-Wall"
      "-fmax-errors=5"
      "-Wno-error=unused-parameter"
      "-Wno-error=parentheses"
      "-fdiagnostics-color"
      "-pedantic"
      "-ggdb3"
      "-D_ANSI_OUTPUT"
      "-Wextra"
      "-Wno-error=implicit-fallthrough"

)

# Ustaw opcje konsolidatora (z raportu Dante)
add_link_options(
)

# Utwórz target 'project1' i dodaj do niego pliki źródlowe:
add_executable(project1
        "translator.c"
        "unit_helper_v2.c"
        "unit_test_v2.c"
        "rdebug.c"
)

# Dołącz niezbędne biblioteki
target_link_libraries(project1
)