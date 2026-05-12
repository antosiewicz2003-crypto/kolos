#include "defs.c"

int __wrap_main(volatile int _argc, char** _argv, char** _envp);

int __real_main(int argc, char** argv, char** envp) {
    (void)argc;
    (void)argv;
    (void)envp;
    return 0;
}

int main(int argc, char** argv, char** envp) {
    return __wrap_main(argc, argv, envp);
}
